// -*- C++ -*-
/*!
 * @file Factory.h
 * @brief generic Factory template class
 * @date $Date$
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2009
 *     Noriaki Ando
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id$
 *
 */

#ifndef COIL_FACTORY_H
#define COIL_FACTORY_H

#include <string>
#include <map>
#include <algorithm>
#include <vector>
#include <coil/Singleton.h>
#include <coil/PeriodicTask.h>
#include <coil/Time.h>

namespace coil
{
  /*!
   * @if jp
   * @brief ctor
   * @else
   * @brief ctor
   * @endif
   */
  PeriodicTask::PeriodicTask()
    : m_period(0.0), m_nowait(false), 
      m_alive(false), m_suspend(false),
      m_execCount(0), m_execCountMax(10),
      m_periodCount(0), m_periodCountMax(10)
  {
  }
  
  /*!
   * @if jp
   * @brief dtor
   * @else
   * @brief dtor
   * @endif
   */
  PeriodicTask::~PeriodicTask()
  {
    finalize();
    wait();
    if (m_func != 0)
      {
        delete m_func;
      }
  }

  /*!
   * @if jp
   * @brief �������¹Ԥ򳫻Ϥ���
   * @else
   * @brief Starting the task
   * @endif
   */
  void PeriodicTask::activate()
  {
    Guard guard(m_alive.mutex);
    if (m_func == 0)   { return; }
    if (m_alive.value) { return; }

    Task::activate();
    m_alive.value = true;
  }

  /*!
   * @if jp
   * @brief �������¹Ԥ�λ����
   * @else
   * @brief Finalizing the task
   * @endif
   */
  void PeriodicTask::finalize()
  {
    Guard gaurd(m_alive.mutex);
    m_alive.value = false;
    this->signal();
  }

  /*!
   * @if jp
   * @brief �������¹Ԥ����Ǥ���
   * @else
   * @brief Suspending the task
   * @endif
   */
  int PeriodicTask::suspend(void)
  {
    Guard guard(m_alive.mutex);
    if (!m_alive.value) { return -1; }

    Guard gaurd(m_suspend.mutex);
    m_suspend.suspend = true;
    return 0;
  }

  /*!
   * @if jp
   * @brief ���Ǥ���Ƥ��륿������Ƴ�����
   * @else
   * @brief Resuming the suspended task
   * @endif
   */
  int PeriodicTask::resume(void)
  {
    Guard guard(m_alive.mutex);
    if (!m_alive.value) { return -1; }

    m_periodTime.reset();
    m_execTime.reset();

    Guard gaurd(m_suspend.mutex);
    m_suspend.suspend = false;
    m_suspend.cond.signal();
    return 0;
  }

  /*!
   * @if jp
   * @brief ���Ǥ���Ƥ��륿������1���������¹Ԥ���
   * @else
   * @brief Executing the suspended task one tick
   * @endif
   */
  void PeriodicTask::signal(void)
  {
    if (!m_alive.value) { return; }
    Guard gaurd(m_suspend.mutex);
    m_suspend.cond.signal();
  }

  /*!
   * @if jp
   * @brief �������¹Դؿ��򥻥åȤ���
   * @else
   * @brief Setting task execution function
   * @endif
   */
  bool PeriodicTask::setTask(TaskFuncBase* func)
  {
    if (func == 0) { return false; }
    m_func = func;
    return true;
  }
  
  /*!
   * @if jp
   * @brief �������¹Լ����򥻥åȤ���
   * @else
   * @brief Setting task execution period
   * @endif
   */
  void PeriodicTask::setPeriod(double period)
  {
    m_period = period;

    if (m_period.sec() == 0 && m_period.usec() == 0)
      {
        m_nowait = true;
        return;
      }
    m_nowait = false;
    return;
  }

  /*!
   * @if jp
   * @brief �������¹Լ����򥻥åȤ���
   * @else
   * @brief Setting task execution period
   * @endif
   */
  void PeriodicTask::setPeriod(TimeValue& period)
  {
    m_period = period;

    if (m_period.sec() == 0 && m_period.usec() == 0)
      {
        m_nowait = true;
        return;
      }
    m_nowait = false;
    return;
  }

  /*!
   * @if jp
   * @brief �������ؿ��¹Ի��ַ�¬��ͭ���ˤ��뤫
   * @else
   * @brief 
   * @endif
   */
  void PeriodicTask::executionMeasure(bool value)
  {
    m_execMeasure = value;
  }

  /*!
   * @if jp
   * @brief �������ؿ��¹Ի��ַ�¬����
   * @else
   * @brief 
   * @endif
   */
  void PeriodicTask::executionMeasureCount(int n)
  {
    m_execCountMax = n;
  }

  /*!
   * @if jp
   * @brief �������������ַ�¬��ͭ���ˤ��뤫
   * @else
   * @brief 
   * @endif
   */
  void PeriodicTask::periodicMeasure(bool value)
  {
    m_periodMeasure = value;
  }

  /*!
   * @if jp
   * @brief �������������ַ�¬����
   * @else
   * @brief 
   * @endif
   */
  void PeriodicTask::periodicMeasureCount(int n)
  {
    m_periodCountMax = n;
  }

  /*!
   * @if jp
   * @brief �������ؿ��¹Ի��ַ�¬��̤����
   * @else
   * @brief 
   * @endif
   */
  TimeMeasure::Statistics PeriodicTask::getExecStat()
  {
    Guard guard(m_execStat.mutex);
    return m_execStat.stat;
  }

  /*!
   * @if jp
   * @brief �������������ַ�¬��̤����
   * @else
   * @brief 
   * @endif
   */
  TimeMeasure::Statistics PeriodicTask::getPeriodStat()
  {
    Guard guard(m_periodStat.mutex);
    return m_periodStat.stat;
  }

  //----------------------------------------------------------------------
  // protected functions
  /*!
   * @if jp
   * @brief
   * @else
   * @brief 
   * @endif
   */
  int PeriodicTask::svc()
  {
    while (m_alive.value)
      {
        if (m_periodMeasure)
          {
            m_periodTime.tack();
            m_periodTime.tick();
          }

        if (m_execMeasure) { m_execTime.tick(); }
        (*m_func)();
        if (m_execMeasure) { m_execTime.tack(); }


        updateExecStat();
        sleep();
        updatePeriodStat();

        Guard gaurd(m_suspend.mutex);
        while (m_suspend.suspend)
          {
            m_suspend.cond.wait();
          }
      }
    return 0;
  }

  //----------------------------------------------------------------------
  // protected member function

  void PeriodicTask::sleep()
  { 
    if (m_nowait)
      {
        return;
      }
    coil::sleep(m_period - m_execTime.interval());
  }

  void PeriodicTask::updateExecStat()
  {
    if (m_execCount > m_execCountMax)
      {
        Guard guard(m_execStat.mutex);
        m_execStat.stat = m_execTime.getStatistics();
        m_execCount = 0;
      }
    ++m_execCount;
  }

  void PeriodicTask::updatePeriodStat()
  {
    if (m_periodCount > m_periodCountMax)
      {
        Guard guard(m_periodStat.mutex);
        m_periodStat.stat = m_periodTime.getStatistics();
        m_periodCount = 0;
      }
    ++m_periodCount;
  }

}; // namespace coil
#endif // COIL_FACTORY_H
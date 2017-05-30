// -*- C++ -*-
/*!
 * @file Time_posix.h
 * @brief Time functions
 * @date $Date$
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2008
 *     Task-intelligence Research Group,
 *     Intelligent Systems Research Institute,
 *     National Institute of
 *         Advanced Industrial Science and Technology (AIST), Japan
 *     All rights reserved.
 *
 * $Id$
 *
 */

#ifndef COIL_TIME_H
#define COIL_TIME_H

#include <unistd.h>
#if defined(VXWORKS_66) && !defined(__RTP__)
#include <timers.h>
#else
#include <sys/time.h>
#endif

#include <sys/types.h>
#include <time.h>
#include <iostream>
#include <coil/config_coil.h>
#include <coil/TimeValue.h>

#ifndef __RTP__
#include <selectLib.h>
#endif

namespace coil
{

  /*!
   * @if jp
   * @brief 指定された秒間は処理を休止する
   *
   * 指定された秒間は処理を休止する。
   *
   * @param seconds 秒数
   *
   * @return 0: 成功, >0: 失敗
   *
   * @else
   * @brief Stop a processing at specified second time
   *
   * Stop a processing at specified second time.
   *
   * @param seconds Second time
   *
   * @return 0: successful, >0: failed
   *
   * @endif
   */
  inline unsigned int sleep(unsigned int seconds)
  {
    return ::sleep(seconds);
  }

  /*!
   * @if jp
   * @brief 指定された秒間は処理を休止する
   *
   * 指定された秒間は処理を休止する。
   *
   * @param interval TimeValueオブジェクト
   *
   * @return 0: 成功, >0: 失敗
   *
   * @else
   * @brief Stop a processing at specified second time
   *
   * Stop a processing at specified second time.
   *
   * @param interval TimeValue object
   *
   * @return 0: successful, >0: failed
   *
   * @endif
   */
  inline int sleep(TimeValue interval)
  {
    timeval tv;
    tv.tv_sec = interval.sec();
    tv.tv_usec = interval.usec();
    return ::select(0, 0, 0, 0, &tv);
  }

  /*!
   * @if jp
   * @brief 指定されたマイクロ秒間は処理を休止する
   *
   * 指定されたマイクロ秒間は処理を休止する。
   *
   * @param usec マイクロ秒数
   *
   * @return 0: 成功, -1: 失敗
   *
   * @else
   * @brief Stop a processing at specified micro second time
   *
   * Stop a processing at specified micro second time.
   *
   * @param usec Micro second time
   *
   * @return 0: successful, -1: failed
   *
   * @endif
   */
  inline int usleep(unsigned int usec)
  {
	  struct timespec req = {0,usec * 1000};
	  return ::nanosleep(&req, NULL);
  }

  /*!
   * @if jp
   * @brief 時刻とタイムゾーンを取得する
   *
   * 時刻とタイムゾーンを取得する。
   *
   * @param tv 時刻構造体
   * @param tz タイムゾーン構造体
   *
   * @return 0: 成功, -1: 失敗
   *
   * @else
   * @brief Get the time and timezone
   *
   * Get the time and timezone
   *
   * @param tv Structure of time
   * @param tz Structure of timezone
   *
   * @return 0: successful, -1: failed
   *
   * @endif
   */
#if defined(VXWORKS_66) && !defined(__RTP__)
  inline int gettimeofday(struct timespec *tv)
  {
    return ::clock_gettime(CLOCK_REALTIME, tv);
  }
#else
  inline int gettimeofday(struct timeval *tv, struct timezone *tz)
  {
    return ::gettimeofday(tv, tz);
  }
#endif

  /*!
   * @if jp
   * @brief 時刻を取得する
   *
   * 時刻を取得する。
   *
   * @return TimeValueオブジェクト
   *
   * @else
   * @brief Get the time
   *
   * Get the time
   *
   * @return TimeValue object
   *
   * @endif
   */
  inline TimeValue gettimeofday()
  {
#if defined(VXWORKS_66) && !defined(__RTP__)
    timespec tv;
    ::clock_gettime(CLOCK_REALTIME, &tv);
    return TimeValue(tv.tv_sec, tv.tv_nsec/1000);
#else
    timeval tv;
    ::gettimeofday(&tv, 0);
    return TimeValue(tv.tv_sec, tv.tv_usec);
#endif
    
  }



  /*!
   * @if jp
   * @brief 時刻とタイムゾーンを設定する
   *
   * 時刻とタイムゾーンを設定する。
   *
   * @param tv 時刻構造体
   * @param tz タイムゾーン構造体
   *
   * @return 0: 成功, -1: 失敗
   *
   * @else
   * @brief Set the time and timezone
   *
   * Set the time and timezone
   *
   * @param tv Structure of time
   * @param tz Structure of timezone
   *
   * @return 0: successful, -1: failed
   *
   * @endif
   */
#if defined(VXWORKS_66) && !defined(__RTP__)
  inline int settimeofday(struct timespec *tv)
  {
    return ::clock_settime(CLOCK_REALTIME, tv);
  }
#else
  inline int settimeofday(struct timeval *tv , struct timezone *tz)
  {
    return ::settimeofday(tv, tz);
  }
#endif


};

#endif // COIL_TIME_H

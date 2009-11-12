// -*- C++ -*-
/*!
 * @file InPortPushConnector.cpp
 * @brief InPortPush type connector class
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

#include <rtm/InPortPushConnector.h>
#include <rtm/InPortProvider.h>

namespace RTC
{
  /*!
   * @if jp
   * @brief コンストラクタ
   * @else
   * @brief Constructor
   * @endif
   */
  InPortPushConnector::InPortPushConnector(ConnectorInfo info, 
                                           InPortProvider* provider,
                                           ConnectorListeners& listeners,
                                           CdrBufferBase* buffer)
    : InPortConnector(info, buffer),
      m_provider(provider), 
      m_listeners(listeners), 
      m_deleteBuffer(buffer == 0 ? true : false)
  {
    // publisher/buffer creation. This may throw std::bad_alloc;
    if (m_buffer == 0)
      {
        m_buffer = createBuffer(info);
      }
    if (m_buffer == 0 || m_provider==0) throw std::bad_alloc();

    m_provider->init(info.properties);
    m_provider->setBuffer(m_buffer);
    m_provider->setListener(info, &m_listeners);
  }

  /*!
   * @if jp
   * @brief デストラクタ
   * @else
   * @brief Destructor
   * @endif
   */
  InPortPushConnector::~InPortPushConnector()
  {
    disconnect();
  }

  /*!
   * @if jp
   * @brief データの読み出し
   * @else
   * @brief Reading data
   * @endif
   */
  ConnectorBase::ReturnCode
  InPortPushConnector::read(cdrMemoryStream& data)
  {
    RTC_TRACE(("read()"));
    /*
     * buffer returns
     *   BUFFER_OK
     *   BUFFER_EMPTY
     *   TIMEOUT
     *   PRECONDITION_NOT_MET
     */
    if (m_buffer == 0)
      {
        return PRECONDITION_NOT_MET;
      }
    BufferStatus::Enum ret = m_buffer->read(data, 0, 0);
    switch (ret)
      {
      case BufferStatus::BUFFER_OK:
        return PORT_OK;
        break;
      case BufferStatus::BUFFER_EMPTY:
        return BUFFER_EMPTY;
        break;
      case BufferStatus::TIMEOUT:
        return BUFFER_TIMEOUT;
        break;
      case BufferStatus::PRECONDITION_NOT_MET:
        return PRECONDITION_NOT_MET;
        break;
      default:
        return PORT_ERROR;
      }
  }

  /*!
   * @if jp
   * @brief 接続解除
   * @else
   * @brief disconnect
   * @endif
   */
  ConnectorBase::ReturnCode InPortPushConnector::disconnect()
  {
    // delete consumer
    if (m_provider != 0)
      {
        InPortProviderFactory& cfactory(InPortProviderFactory::instance());
        cfactory.deleteObject(m_provider);
      }
    m_provider = 0;

    // delete buffer
    if (m_buffer != 0 && m_deleteBuffer == true)
      {
        CdrBufferFactory& bfactory(CdrBufferFactory::instance());
        bfactory.deleteObject(m_buffer);
      }
    m_buffer = 0;

    return PORT_OK;
  }
  
  /*!
   * @if jp
   * @brief Bufferの生成
   * @else
   * @brief create buffer
   * @endif
   */
  CdrBufferBase* InPortPushConnector::createBuffer(ConnectorInfo& info)
  {
    std::string buf_type;
    buf_type = info.properties.getProperty("buffer_type",
                                              "ring_buffer");
    return CdrBufferFactory::instance().createObject(buf_type);
  }

};


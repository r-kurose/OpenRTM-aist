// -*- C++ -*-
/*!
 * @file OutPortBase.h
 * @brief InPortBase base class
 * @date $Date: 2007-12-31 03:08:05 $
 * @author Noriaki Ando <n-ando@aist.go.jp>
 *
 * Copyright (C) 2003-2008
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

#ifndef RTC_OUTPORTBASE_H
#define RTC_OUTPORTBASE_H

#include <vector>
#include <string>

#include <coil/Properties.h>
#include <coil/stringutil.h>

#include <rtm/PortBase.h>
#include <rtm/InPortConsumer.h>
#include <rtm/OutPortProvider.h>
#include <rtm/ConnectorBase.h>
#include <rtm/CdrBufferBase.h>
#include <rtm/SystemLogger.h>
#include <rtm/ConnectorListener.h>

namespace RTC
{
  class PublisherBase;
  class ConnectorBase;
  class OutPortConnector;

  /*!
   * @if jp
   *
   * @class OutPortBase
   *
   * @brief OutPort 基底クラス
   * 
   * OutPort の基底クラス。
   *
   *
   *
   * Properties: port.outport
   * プロパティは
   *
   * - port.outport
   * - port.outport.[name]
   * ConnectorProfile.properties の場合は
   * - dataport.outport
   *
   * 以下に指定したものが渡される。
   * (port.outport.[name]が優先される)
   * さらに、一部のプロパティは接続時に ConnectorProfile により
   * 渡される場合があり、その場合は ConnectorProfile が優先される。
   *
   * - input.throughput.profile: enable
   * - input.throughput.update_rate: count [n/count]
   * - input.throughput.total_bytes: [bytes]
   * - input.throughput.total_count: [n]
   * - input.throughput.max_size: [bytes]
   * - input.throughput.min_size: [bytes]
   * - input.throughput.avg_size: [bytes]
   * - input.throughput.byte_sec: [bytes/sec]
   *
   * - output.throughput.profile: enable
   * - output.throughput.update_rate: count [n/count]
   * - output.throughput.total_bytes: [bytes]
   * - output.throughput.total_count:[n]
   * - output.throughput.max_size: [bytes]
   * - output.throughput.min_size: [bytes]
   * - output.throughput.avg_size: [bytes]
   * - output.throughput.max_sendtime: [sec]
   * - output.throughput.min_sendtime: [sec]
   * - output.throughput.avg_sendtime: [sec]
   * - output.throughput.byte_sec: [bytes/sec]
   *
   * dataport.dataflow_type
   * dataport.interface_type
   * dataport.subscription_type
   *
   * [buffer]
   *
   * - buffer.type:
   *     利用可能なバッファのタイプ
   *     ConnectorProfile の場合は利用するバッファのタイプ
   *     無指定の場合はデフォルトの ringbuffer が使用される。
   *     ex. ringbuffer, shmbuffer, doublebuffer, etc.
   *     正し、Consumer, Publisher のタイプによっては特定のバッファ型を
   *     要求するがあるための、その場合は指定は無効となる。
   *
   * - buffer.length:
   *     バッファの長さ
   *
   * - buffer.write.full_policy:
   *     上書きするかどうかのポリシー
   *     overwrite (上書き), do_nothing (何もしない), block (ブロックする)
   *     block を指定した場合、次の timeout 値を指定すれば、指定時間後
   *     書き込み不可能であればタイムアウトする。
   *
   * - buffer.write.timeout:
   *     タイムアウト時間を [sec] で指定する。
   *     1 sec -> 1.0, 1 ms -> 0.001, タイムアウトしない -> 0.0
   *
   * - buffer.read.empty_policy:
   *     バッファが空のときの読み出しポリシー
   *     last (最後の要素), do_nothing (何もしない), block (ブロックする)
   *     block を指定した場合、次の timeout 値を指定すれば、指定時間後
   *     読み出し不可能であればタイムアウトする。
   *
   * - buffer.read.timeout:
   *     タイムアウト時間 [sec] で指定する。
   *     1sec -> 1.0, 1ms -> 0.001, タイムアウトしない -> 0.0
   *
   * - その他バッファ毎に固有なオプション
   *
   *
   * [publihser]
   *
   * - publisher.types:
   *      利用可能な Publisher のタイプ
   *      new, periodic, flush, etc..
   *
   * - publisher.push.policy:
   *      InPortへデータを送信するポリシー
   *      all: バッファにたまっているすべて送信
   *      fifo: バッファをFIFOとみなして送信
   *      skip: 古いデータから一定数を間引いて送信
   *      new: 常に新しいデータのみを送信
   *
   * - publisher.push.skip_rate:
   *      push.policy=skip のときのスキップ率
   *      n: n要素毎にひとつ送信
   *
   * - publisher.periodic.rate:
   *
   * - publisher.thread.type:
   *       Publisher のスレッドのタイプ
   * - publisher.thread.measurement.exec_time: yes/no
   * - publisher.thread.measurement.exec_count: number
   * - publisher.thread.measurement.period_time: yes/no
   * - publisher.thread.measurement.period_count: number
   *
   * [interface]
   *
   * - interface.types:
   *     OutPort interfaceのタイプ
   *     ex. corba_cdr, corba_any, raw_tcp などカンマ区切りで指定。何も
   *     指定しなければ利用可能なすべてのプロバイダが使用される
   *
   *
   *
   *   
   * OutPort 側の connect() では以下のシーケンスで処理が行われる。
   *
   * 1. OutPort に関連する connector 情報の生成およびセット
   *
   * 2. InPortに関連する connector 情報の取得
   *  - ConnectorProfile::properties["dataport.corba_any.inport_ref"]に
   *    OutPortAny のオブジェクトリファレンスが設定されている場合、
   *    リファレンスを取得してConsumerオブジェクトにセットする。
   *    リファレンスがセットされていなければ無視して継続。
   *    (OutPortがconnect() 呼び出しのエントリポイントの場合は、
   *    InPortのオブジェクトリファレンスはセットされていないはずである。)
   * 3. PortBase::connect() をコール
   *    Portの接続の基本処理が行われる。
   * 4. 上記2.でInPortのリファレンスが取得できなければ、再度InPortに
   *    関連する connector 情報を取得する。
   *
   * 5. ConnectorProfile::properties で与えられた情報から、
   *    OutPort側の初期化処理を行う。
   *
   * - [dataport.interface_type]
   * -- CORBA_Any の場合: 
   *    InPortAny を通してデータ交換される。
   *    ConnectorProfile::properties["dataport.corba_any.inport_ref"]に
   *    InPortAny のオブジェクトリファレンスをセットする。
   * -- RawTCP の場合: Raw TCP socket を通してデータ交換される。
   *    ConnectorProfile::properties["dataport.raw_tcp.server_addr"]
   *    にInPort側のサーバアドレスをセットする。
   *
   * - [dataport.dataflow_type]
   * -- Pushの場合: Subscriberを生成する。Subscriberのタイプは、
   *    dataport.subscription_type に設定されている。
   * -- Pullの場合: InPort側がデータをPull型で取得するため、
   *    特に何もする必要が無い。
   *
   * - [dataport.subscription_type]
   * -- Onceの場合: SubscriberOnceを生成する。
   * -- Newの場合: SubscriberNewを生成する。
   * -- Periodicの場合: SubscriberPeriodicを生成する。
   *
   * - [dataport.publisher.push_rate]
   * -- dataport.subscription_type=Periodicの場合周期を設定する。
   *
   * 6. 上記の処理のうち一つでもエラーであれば、エラーリターンする。
   *    正常に処理が行われた場合はRTC::RTC_OKでリターンする。
   *
   * @since 0.2.0
   *
   * @else
   *
   * @class OutPortBase
   *
   * @brief Output base class.
   *
   * The base class of OutPort<T> which are implementations of OutPort
   *
   * Form a kind of Observer pattern with OutPortBase and PublisherBase.
   * attach(), detach(), notify() of OutPortBase and
   * push() of PublisherBase are methods associated with the Observer pattern.
   *
   * @since 0.2.0
   *
   * @endif
   */
  class OutPortBase
    : public PortBase, public DataPortStatus
  {
  public:
    DATAPORTSTATUS_ENUM

    typedef std::vector<OutPortConnector*> ConnectorList;

    /*!
     * @if jp
     * @brief コンストラクタ
     *
     * コンストラクタ。
     *
     * @param name ポート名
     *
     * @else
     * @brief Constructor
     *
     * Constructor
     *
     * @param name Port name
     *
     * @endif
     */
    OutPortBase(const char* name, const char* data_type);
    
    /*!
     * @if jp
     * @brief デストラクタ
     *
     * デストラクタ。
     * 登録された全ての Publisher を削除する。
     *
     * @else
     *
     * @brief Destructor
     *
     * Destructor
     * Detach all subscribed Publishers.
     *
     * @endif
     */
    virtual ~OutPortBase(void);
    
    /*!
     * @if jp
     * @brief プロパティの初期化
     *
     * OutPortのプロパティを初期化する
     *
     * @else
     *
     * @brief Initializing properties
     *
     * This operation initializes outport's properties
     *
     * @endif
     */
    void init(coil::Properties& prop);

    /*!
     * @if jp
     * @brief OutPort名称の取得
     *
     * OutPortの名称を取得する。
     *
     * @return ポート名称
     *
     * @else
     *
     * @brief Retrieve OutPort name
     *
     * Retrieve OutPort name
     *
     * @return the port name
     *
     * @endif
     */
    const char* name() const;

    /*!
     * @if jp
     * @brief プロパティを取得する
     *
     * OutPortのプロパティを取得する。
     *
     * @return プロパティ
     *
     * @else
     *
     * @brief Get properties
     *
     * Getting properties of this OutPort
     *
     * @return OutPort's properties
     *
     * @endif
     */
    coil::Properties& properties();

    /*!
     * @if jp
     * @brief Connector を取得
     *
     * 現在所有しているコネクタを取得する。
     *
     * @return connector のリスト
     *
     * @else
     *
     * @brief Connector list
     *
     * This operation returns connector list
     *
     * @return connector list
     *
     * @endif
     */
    const std::vector<OutPortConnector*>& connectors();

    /*!
     * @if jp
     * @brief ConnectorProfile を取得
     *
     * 現在所有しているコネクタのProfileを取得する。
     *
     * @return ConnectorProfile のリスト
     *
     * @else
     *
     * @brief ConnectorProfile list
     *
     * This operation returns ConnectorProfile list
     *
     * @return connector list
     *
     * @endif
     */
    ConnectorInfoList getConnectorProfiles();

    /*!
     * @if jp
     * @brief ConnectorId を取得
     *
     * 現在所有しているコネクタのIDを取得する。
     *
     * @return ConnectorId のリスト
     *
     * @else
     *
     * @brief ConnectorId list
     *
     * This operation returns ConnectorId list
     *
     * @return connector list
     *
     * @endif
     */
    coil::vstring getConnectorIds();

    /*!
     * @if jp
     * @brief Connectorの名前を取得
     *
     * 現在所有しているコネクタの名前を取得する。
     *
     * @return Connector名のリスト
     *
     * @else
     *
     * @brief Connector name list
     *
     * This operation returns Connector name list
     *
     * @return connector name list
     *
     * @endif
     */
    coil::vstring getConnectorNames();

    /*!
     * @if jp
     * @brief ConnectorProfileをIDで取得
     *
     * 現在所有しているコネクタをIDで取得する。
     *
     * @param id Connector ID
     * @param prof ConnectorProfile
     * @return false 指定したIDがない
     *
     * @else
     *
     * @brief Getting ConnectorProfile by name
     *
     * This operation returns ConnectorProfile specified by name
     *
     * @param id Connector ID
     * @param prof ConnectorProfile
     * @return false　specified ID does not exist
     *
     * @endif
     */
    bool getConnectorProfileById(const char* id,
                                 ConnectorInfo& prof);

    /*!
     * @if jp
     * @brief ConnectorProfileを名前で取得
     *
     * 現在所有しているコネクタを名前で取得する。
     *
     * @param name Connector name
     * @param prof ConnectorProfile
     * @return false 指定した名前がない
     *
     * @else
     *
     * @brief Getting ConnectorProfile by name
     *
     * This operation returns ConnectorProfile specified by name
     *
     * @param id Connector ID
     * @param prof ConnectorProfile
     * @return false specified name does not exist
     *
     * @endif
     */
    bool getConnectorProfileByName(const char* name,
                                   ConnectorInfo& prof);

    /*!
     * @if jp
     * @brief インターフェースプロファイルを公開する
     *
     * 現在 OutPort が所有しているインターフェースのプロファイルを
     * DataOutPortに対して公開するためのオペレーション。
     * DataOutPortから呼ばれることを意図している
     *
     * @param properties IFプロファイルを書き込むNVList
     *
     * @else
     *
     * @brief Publish interface profile
     *
     * This operation publish interface profiles of this OutPort
     * to DataOutPort. This operation should be called from DataOutPort.
     *
     * @param properties itnerface profile
     *
     * @endif
     */
    bool publishInterfaceProfiles(SDOPackage::NVList& properties);

    /*!
     * @if jp
     * @brief Interface情報を公開する
     *
     * Interface情報を公開する。
     *
     * @param connector_profile Interface情報を書き込むコネクタプロファイル
     *
     * @else
     * @brief Publish interface information
     *
     * Publish interface information.
     *
     * @param connector_profile The connector profile in which
     *                          interface information is written
     *
     * @endif
     */
    //    bool publishInterfaces(ConnectorProfile& connector_profile);

    /*!
     * @if jp
     * @brief Interface情報を取得する
     *
     * コネクタプロファイルで与えられた情報から、インターフェース情報を
     * 取得する。
     *
     * @param connector_profile インターフェース情報を取得するコネクタ
     *                          プロファイル
     *
     * @return 登録処理結果
     *
     * @else
     * @brief Subscribe interface
     *
     * This operation subscribe provided interface in the ConnectorProfile.
     *
     * @param connector_profile The ConnectorProfile that includes
     *                          provided interface information
     *
     * @return Subscription result
     *
     * @endif
     */
    //    bool subscribeInterfaces(const ConnectorProfile& connector_profile);

    /*!
     * @if jp
     * @brief 登録されているInterface情報を解除する
     *
     * 接続を開場するために、登録済みのコネクタプロファイルを解除する。
     * 取得する。
     *
     * @param connector_profile インターフェース情報を解除するコネクタ
     *                          プロファイル
     *
     * @return 登録処理結果
     *
     * @else
     * @brief Unsubscribe interface
     *
     * This operation unsubscribe provided interface in the ConnectorProfile.
     *
     * @param connector_profile The ConnectorProfile of unsubscribed connection
     *
     * @return Subscription result
     *
     * @endif
     */
    //    bool unsubscribeInterfaces(const ConnectorProfile& connector_profile);


    virtual void onConnect(const char* id, PublisherBase* publisher){};
    virtual void onDisconnect(const char* id){};
    
    /*!
     * @if jp
     *
     * @brief InPortを activates する
     *
     * InPortを activate する。
     *
     * @else
     *
     * @brief Activate all Port interfaces
     *
     * This operation activate all interfaces that is registered in the
     * ports.
     *
     * @endif
     */
    virtual void activateInterfaces();

    /*!
     * @if jp
     *
     * @brief 全ての Port のインターフェースを deactivates する
     *
     * Port に登録されている全てのインターフェースを deactivate する。
     *
     * @else
     *
     * @brief Deactivate all Port interfaces
     *
     * This operation deactivate all interfaces that is registered in the
     * ports.
     *
     * @endif
     */
    virtual void deactivateInterfaces();


    /*!
     * @if jp
     * @brief ConnectorDataListener リスナを追加する
     *
     * バッファ書き込みまたは読み出しイベントに関連する各種リスナを設定する。
     *
     * 設定できるリスナのタイプとコールバックイベントは以下の通り
     *
     * - ON_BUFFER_WRITE:          バッファ書き込み時
     * - ON_BUFFER_FULL:           バッファフル時
     * - ON_BUFFER_WRITE_TIMEOUT:  バッファ書き込みタイムアウト時
     * - ON_BUFFER_OVERWRITE:      バッファ上書き時
     * - ON_BUFFER_READ:           バッファ読み出し時
     * - ON_SEND:                  InProtへの送信時
     * - ON_RECEIVED:              InProtへの送信完了時
     * - ON_SEND_ERTIMEOUT:        OutPort側タイムアウト時
     * - ON_SEND_ERERROR:          OutPort側エラー時
     * - ON_RECEIVER_FULL:         InProt側バッファフル時
     * - ON_RECEIVER_TIMEOUT:      InProt側バッファタイムアウト時
     * - ON_RECEIVER_ERROR:        InProt側エラー時
     *
     * リスナは ConnectorDataListener を継承し、以下のシグニチャを持つ
     * operator() を実装している必要がある。
     *
     * ConnectorDataListener::
     *         operator()(const ConnectorProfile&, const cdrStream&)
     *
     * デフォルトでは、この関数に与えたリスナオブジェクトの所有権は
     * OutPortに移り、OutPort解体時もしくは、
     * removeConnectorDataListener() により削除時に自動的に解体される。
     * リスナオブジェクトの所有権を呼び出し側で維持したい場合は、第3引
     * 数に false を指定し、自動的な解体を抑制することができる。
     *
     * @param listener_type リスナタイプ
     * @param listener リスナオブジェクトへのポインタ
     * @param autoclean リスナオブジェクトの自動的解体を行うかどうかのフラグ
     *
     * @else
     * @brief Adding BufferDataListener type listener
     *
     * This operation adds certain listeners related to buffer writing and
     * reading events.
     * The following listener types are available.
     *
     * - ON_BUFFER_WRITE:          At the time of buffer write
     * - ON_BUFFER_FULL:           At the time of buffer full
     * - ON_BUFFER_WRITE_TIMEOUT:  At the time of buffer write timeout
     * - ON_BUFFER_OVERWRITE:      At the time of buffer overwrite
     * - ON_BUFFER_READ:           At the time of buffer read
     * - ON_SEND:                  At the time of sending to InPort
     * - ON_RECEIVED:              At the time of finishing sending to InPort
     * - ON_SENDER_TIMEOUT:        At the time of timeout of OutPort
     * - ON_SENDER_ERROR:          At the time of error of OutPort
     * - ON_RECEIVER_FULL:         At the time of bufferfull of InPort
     * - ON_RECEIVER_TIMEOUT:      At the time of timeout of InPort
     * - ON_RECEIVER_ERROR:        At the time of error of InPort
     *
     * Listeners should have the following function operator().
     *
     * ConnectorDataListener::
     *         operator()(const ConnectorProfile&, const cdrStream&)
     *
     * The ownership of the given listener object is transferred to
     * this OutPort object in default.  The given listener object will
     * be destroied automatically in the OutPort's dtor or if the
     * listener is deleted by removeConnectorDataListener() function.
     * If you want to keep ownership of the listener object, give
     * "false" value to 3rd argument to inhibit automatic destruction.
     *
     * @param listener_type A listener type
     * @param listener A pointer to a listener object
     * @param autoclean A flag for automatic listener destruction
     *
     * @endif
     */
    void addConnectorDataListener(ConnectorDataListenerType listener_type,
                                  ConnectorDataListener* listener,
                                  bool autoclean = true);


    /*!
     * @if jp
     * @brief ConnectorDataListener リスナを削除する
     *
     * 設定した各種リスナを削除する。
     * 
     * @param listener_type リスナタイプ
     * @param listener リスナオブジェクトへのポインタ
     *
     * @else
     * @brief Removing BufferDataListener type listener
     *
     * This operation removes a specified listener.
     *     
     * @param listener_type A listener type
     * @param listener A pointer to a listener object
     *
     * @endif
     */
    void removeConnectorDataListener(ConnectorDataListenerType listener_type,
                                     ConnectorDataListener* listener);
    

    /*!
     * @if jp
     * @brief ConnectorListener リスナを追加する
     *
     * バッファ書き込みまたは読み出しイベントに関連する各種リスナを設定する。
     *
     * 設定できるリスナのタイプは
     *
     * - ON_BUFFER_EMPTY:       バッファが空の場合
     * - ON_BUFFER_READTIMEOUT: バッファが空でタイムアウトした場合
     *
     * リスナは以下のシグニチャを持つ operator() を実装している必要がある。
     *
     * ConnectorListener::operator()(const ConnectorProfile&)
     *
     * デフォルトでは、この関数に与えたリスナオブジェクトの所有権は
     * OutPortに移り、OutPort解体時もしくは、
     * removeConnectorListener() により削除時に自動的に解体される。
     * リスナオブジェクトの所有権を呼び出し側で維持したい場合は、第3引
     * 数に false を指定し、自動的な解体を抑制することができる。
     *
     * @param listener_type リスナタイプ
     * @param listener リスナオブジェクトへのポインタ
     * @param autoclean リスナオブジェクトの自動的解体を行うかどうかのフラグ
     *
     * @else
     * @brief Adding ConnectorListener type listener
     *
     * This operation adds certain listeners related to buffer writing and
     * reading events.
     * The following listener types are available.
     *
     * - ON_BUFFER_EMPTY:       At the time of buffer empty
     * - ON_BUFFER_READTIMEOUT: At the time of buffer read timeout
     *
     * Listeners should have the following function operator().
     *
     * ConnectorListener::operator()(const ConnectorProfile&)
     *  
     * The ownership of the given listener object is transferred to
     * this OutPort object in default.  The given listener object will
     * be destroied automatically in the OutPort's dtor or if the
     * listener is deleted by removeConnectorListener() function.
     * If you want to keep ownership of the listener object, give
     * "false" value to 3rd argument to inhibit automatic destruction.
     *
     * @param listener_type A listener type
     * @param listener A pointer to a listener object
     * @param autoclean A flag for automatic listener destruction
     *
     * @endif
     */
    void addConnectorListener(ConnectorListenerType callback_type,
                              ConnectorListener* listener,
                              bool autoclean = true);

    /*!
     * @if jp
     * @brief ConnectorDataListener リスナを削除する
     *
     * 設定した各種リスナを削除する。
     * 
     * @param listener_type リスナタイプ
     * @param listener リスナオブジェクトへのポインタ
     *
     * @else
     * @brief Removing BufferDataListener type listener
     *
     * This operation removes a specified listener.
     *     
     * @param listener_type A listener type
     * @param listener A pointer to a listener object
     *
     * @endif
     */
    void removeConnectorListener(ConnectorListenerType callback_type,
                                 ConnectorListener* listener);

    /*!
     * @if jp
     * @brief endian 設定がlittleか否か返す
     *
     * endian 設定がlittleか否か返す。
     *
     * @return m_endian がlittleの場合true、以外はfalse を返す。
     *
     * @else
     * @brief
     *
     * return it whether endian setting is little.
     *
     *@return Return true in the case of "little", false other than it.
     *
     * @endif
     */
    bool isLittleEndian();


  protected:
    /*!
     * @if jp
     * @brief OutPortの設定を行う
     *
     * propertiesの情報に基づきOutPortの各種設定を行う
     *
     * @else
     *
     * @brief Configureing outport
     *
     * This operation configures the outport based on the properties.
     *
     * @endif
     */
    void configure();

    /*!
     * @if jp
     *
     * @brief Interface 情報を公開する
     *
     * このオペレーションは、notify_connect() 処理シーケンスの始めにコール
     * される純粋仮想関数である。
     * notify_connect() では、
     *
     * - publishInterfaces()
     * - connectNext()
     * - subscribeInterfaces()
     * - updateConnectorProfile()
     *
     * の順に protected 関数がコールされ接続処理が行われる。
     * <br>
     * このオペレーションは、新規の connector_id に対しては接続の生成、
     * 既存の connector_id に対しては更新が適切に行われる必要がある。
     *
     * @param connector_profile 接続に関するプロファイル情報
     *
     * @return ReturnCode_t 型のリターンコード
     *
     * @else
     *
     * @brief Publish interface information
     *
     * This operation is pure virutal function that would be called at the
     * beginning of the notify_connect() process sequence.
     * In the notify_connect(), the following methods would be called in order.
     *
     * - publishInterfaces()
     * - connectNext()
     * - subscribeInterfaces()
     * - updateConnectorProfile() 
     *
     * This operation should create the new connection for the new
     * connector_id, and should update the connection for the existing
     * connection_id.
     *
     * @param connector_profile The connection profile information
     *
     * @return The return code of ReturnCode_t type.
     *
     * @endif
     */
    virtual ReturnCode_t
    publishInterfaces(ConnectorProfile& connector_profile);
    
    /*! @if jp
     *
     * @brief Interface に接続する
     *
     * このオペレーションは、notify_connect() 処理シーケンスの中間にコール
     * される純粋仮想関数である。
     * notify_connect() では、
     *
     * - publishInterfaces()
     * - connectNext()
     * - subscribeInterfaces()
     * - updateConnectorProfile()
     *
     * の順に protected 関数がコールされ接続処理が行われる。
     *
     * @param connector_profile 接続に関するプロファイル情報
     *
     * @return ReturnCode_t 型のリターンコード
     *
     * @else
     *
     * @brief Subscribe to the interface
     *
     * This operation is pure virutal function that would be called at the
     * middle of the notify_connect() process sequence.
     * In the notify_connect(), the following methods would be called in order.
     *
     * - publishInterfaces()
     * - connectNext()
     * - subscribeInterfaces()
     * - updateConnectorProfile()
     *
     * @param connector_profile The connection profile information
     *
     * @return The return code of ReturnCode_t type.
     *
     * @endif
     */
    virtual ReturnCode_t
    subscribeInterfaces(const ConnectorProfile& connector_profile);
    
    /*!
     * @if jp
     *
     * @brief Interface の接続を解除する
     *
     * このオペレーションは、notify_disconnect() 処理シーケンスの終わりにコール
     * される純粋仮想関数である。
     * notify_disconnect() では、
     * - disconnectNext()
     * - unsubscribeInterfaces()
     * - eraseConnectorProfile()
     * の順に protected 関数がコールされ接続解除処理が行われる。
     *
     * @param connector_profile 接続に関するプロファイル情報
     *
     * @else
     *
     * @brief Disconnect the interface connection
     *
     * This operation is pure virutal function that would be called at the
     * end of the notify_disconnect() process sequence.
     * In the notify_disconnect(), the following methods would be called.
     * - disconnectNext()
     * - unsubscribeInterfaces()
     * - eraseConnectorProfile() 
     *
     * @param connector_profile The profile information associated with 
     *                          the connection
     *
     * @endif
     */
    virtual void
    unsubscribeInterfaces(const ConnectorProfile& connector_profile);

    /*!
     * @if jp
     * @brief OutPort provider の初期化
     * @else
     * @brief OutPort provider initialization
     * @endif
     */
    void initProviders();

    /*!
     * @if jp
     * @brief InPort consumer の初期化
     * @else
     * @brief InPort consumer initialization
     * @endif
     */
    void initConsumers();

    /*!
     * @if jp
     * @brief OutPort provider の生成
     * @else
     * @brief OutPort provider creation
     * @endif
     */
    OutPortProvider* createProvider(ConnectorProfile& cprof,
                                    coil::Properties& prop);
    /*!
     * @if jp
     * @brief InPort consumer の生成
     * @else
     * @brief InPort consumer creation
     * @endif
     */
    InPortConsumer* createConsumer(const ConnectorProfile& cprof,
                                   coil::Properties& prop);
    
    /*!
     * @if jp
     * @brief OutPortPushConnector の生成
     * @else
     * @brief OutPortPushConnector creation
     * @endif
     */
    OutPortConnector* createConnector(const ConnectorProfile& cprof,
                                      coil::Properties& prop,
                                      InPortConsumer* consumer);
    /*!
     * @if jp
     * @brief OutPortPullConnector の生成
     * @else
     * @brief OutPortPullConnector creation
     * @endif
     */
    OutPortConnector* createConnector(const ConnectorProfile& cprof,
                                      coil::Properties& prop,
                                      OutPortProvider* provider);

  protected:
    coil::Properties m_properties;
    std::string m_name;
    std::vector<OutPortConnector*> m_connectors;
    std::vector<InPortConsumer*> m_consumers;
    coil::vstring m_providerTypes;
    coil::vstring m_consumerTypes;
    std::vector<OutPortProvider*> m_providers;
    std::string m_endian;

    ConnectorListeners m_listeners;
   
    /*!
     * @if jp
     * @brief provider を削除するための Functor
     * @else
     * @brief Functor to delete providers
     * @endif
     */
    struct provider_cleanup;

    /*!
     * @if jp
     * @brief connector を削除するための Functor
     * @else
     * @brief Functor to delete connectors
     * @endif
     */
    struct connector_cleanup;
  };
}; // End of namespace RTM
#endif // RTC_RTCOUTPORTBASE_H

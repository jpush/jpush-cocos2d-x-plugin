//  JPushService.h
//  PushSDK
//
//  Created by zhanghao on 14-4-16.
//  Copyright (c) 2012年 HXHG. All rights reserved.
//
#ifndef JPUSHSERVIECE_H
#define JPUSHSERVIECE_H

#include <string>
#include <map>
#include <set>
using namespace std;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
typedef set<string> *c_tags;
typedef map<string, void *> *message;
/*
 *  IOS回调函数标准格式(传给SDK用来回调）
 */
typedef void (*APNetworkDidSetup_callback)(void *p_handle);
typedef void (*APNetworkDidClose_callback)(void *p_handle);
typedef void (*APNetworkDidRegister_callback)(void *p_handle,
                                              const char *registrationID);
typedef void (*APNetworkDidLogin_callback)(void *p_handle);
typedef void (*APNetworkDidReceiveMessage_callback)(
    void *p_handle, map<string, void *> *message);
#endif

typedef void (*APTagAliasCallback)(void *p_handle, int responseCode,
                                   const char *alias, set<string> *tags);
class JPushService {
 public:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
  /*
  *   设置SDK常用的回调函数(调用该函数就无需分别调用每个回调的接口）:
  *   如果不需要回调函数，传0或者NULL参数
  *   setup_callback     建立连接回调函数
  *   close_callback     关闭连接回调函数
  *   register_callback  注册成功回调函数
  *   login_callback     登录成功回调函数
  *   message_callback   收到消息(非APNS)回调函数
  */
  static void registerCallbackFunction(
      void *p_handle, APNetworkDidSetup_callback setup_callback,
      APNetworkDidClose_callback close_callback,
      APNetworkDidRegister_callback register_callback,
      APNetworkDidLogin_callback login_callback,
      APNetworkDidReceiveMessage_callback message_callback);

  /*
   *   设置SDK常用的回调函数(分别在需要地方调用每个回调接口）:
   *   registerSetupCallbackFunction     建立连接回调函数
   *   registerCloseCallbackFunction     关闭连接回调函数
   *   registerRegisterCallbackFunction  注册成功回调函数
   *   registerLoginCallbackFunction     登录成功回调函数
   *   registerCallbackFunction   收到消息(非APNS)回调函数
   */
  static void registerSetupCallbackFunction(
      void *p_handle, APNetworkDidSetup_callback setup_callback);
  static void registerCloseCallbackFunction(
      void *p_handle, APNetworkDidClose_callback close_callback);
  static void registerRegisterCallbackFunction(
      void *p_handle, APNetworkDidRegister_callback register_callback);
  static void registerLoginCallbackFunction(
      void *p_handle, APNetworkDidLogin_callback login_callback);
  static void registerCallbackFunction(
      void *p_handle, APNetworkDidReceiveMessage_callback message_callback);

  /**
   *  记录页面停留时间功能。
   *  pageStart和pageEnd为自动计算停留时间
   *  pageDefine为手动自己输入停留时间
   *
   *  @param pageName 页面名称
   *  @param seconds  页面停留时间
   */
  static void pageStart(const char *pageName);
  static void pageEnd(const char *pageName);
  static void pageDefine(const char *pageName, int duration);

  /**
   *  get the UDID
   */
  static const char *openUDID();  // UDID
#endif
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
  //注册，推送
  static void init();
  static void setDebugMode(bool enable);
  //停止与恢复推送服务API
  static void stopPush();
  static void resumePush();
  static bool isPushStopped();
  //清除通知API
  static void clearAllNotifications();
  static void clearNotificationById(int notificationId);
  //设置允许推送时间 API
  static void setPushTime(set<int> *weekDays, int startHour, int endHour);
  //设置通知静默时间 API
  static void setSilenceTime(int startHour, int startMinute, int endHour,
                             int endMinute);
  //设置保留最近通知条数 API
  static void setLatestNotifactionNumber(int maxNum);
#endif

  /**
   *  Tags & Alias
   *
   *下面的接口是可选的
   * 设置标签和(或)别名（若参数为nil，则忽略；若是空对象，则清空；详情请参考文档：http://docs.jpush.cn/pages/viewpage.action?pageId=3309913）
   */
  static void setAliasAndTags(void *p_handle, const char *alias,
                              set<string> *tags, APTagAliasCallback callback);
  static void setAlias(void *p_handle, const char *alias,
                       APTagAliasCallback callback);
  static void setTags(void *p_handle, set<string> *tags,
                      APTagAliasCallback callback);

  /**
   * 用于过滤出正确可用的tags，如果总数量超出最大限制则返回最大数量的靠前的可用tags
   */
  static bool filterValidTags(set<string> *tags, set<string> *result);

  /**
   *  get RegistrationID
   */
  static const char *registrationID();
};

#endif
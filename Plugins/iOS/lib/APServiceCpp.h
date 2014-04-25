//
//  APServiceCpp.h
//  PushSDK
//
//  Created by 张 on 14-4-16.
//
//
#ifndef PushSDK_APServiceCpp_h
#define PushSDK_APServiceCpp_h

#include <string>
#include <map>
#include <set>
#import "APService.h"

using namespace std;

typedef set<string> *c_tags;
typedef map<string, void *> message;
typedef void (*tags_alias_callback)(int iResCode, c_tags tags,
                                    const char *alias);
typedef void (*networkDidSetup_callback)();
typedef void (*networkDidClose_callback)();
typedef void (*networkDidRegister_callback)(const char *registrationID);
typedef void (*networkDidLogin_callback)();
typedef void (*networkDidReceiveMessage_callback)(message message);

class APServiceCpp {
 public:
  /**
   *  初始化
   */
  //#if defined(IOS)
  // 以下四个接口是必须调用的
  static void setupWithOption(NSDictionary *launchingOption);
  // 初始化
  static void registerForRemoteNotificationTypes(int types);  // 注册APNS类型
  static void registerDeviceToken(NSData *deviceToken);
  // 向服务器上报Device Token
  static void handleRemoteNotification(
      NSDictionary *
          remoteInfo);  // 处理收到的APNS消息，向服务器上报收到APNS消息
                        /*
*   设置SDK常用的回调函数:
*   如果不需要回调函数，传0或者NULL参数
*   setup_callback     建立连接回调函数
*   close_callback     关闭连接回调函数
*   register_callback  注册成功回调函数
*   login_callback     登录成功回调函数
*   message_callback   收到消息(非APNS)回调函数
*/
  static void setCallbackFunction(
      networkDidSetup_callback setup_callback,
      networkDidClose_callback close_callback,
      networkDidRegister_callback register_callback,
      networkDidLogin_callback login_callback,
      networkDidReceiveMessage_callback message_callback);
  //#else
  static void start();
  //#endif

  /**
   *  Tags & Alias
   *
   *下面的接口是可选的
   * 设置标签和(或)别名（若参数为nil，则忽略；若是空对象，则清空；详情请参考文档：http://docs.jpush.cn/pages/viewpage.action?pageId=3309913）
   */
  static void setTags(c_tags tags, const char *alias,
                      tags_alias_callback callback);
  static void setTags(c_tags tags, tags_alias_callback callback);
  static void setTags(const char *alias, tags_alias_callback callback);

  ////
  ///用于过滤出正确可用的tags，如果总数量超出最大限制则返回最大数量的靠前的可用tags
  static c_tags filterValidTags(c_tags *tags);

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
  static const char *openUDID() DEPRECATED_ATTRIBUTE;  // UDID

  /**
   *  get RegistionID
   */
  static const char *registrionID();
};

#endif
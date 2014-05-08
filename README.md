jpush-cocos2d-x-plugin
======================

JPush's officially supported Cocos2d-x plugin (Android &amp; iOS). 极光推送官方支持的 Cocos2d-x 插件（Android &amp; iOS）。

## 导入到 Cocos2d-x 项目

* 搭建好Cocos2d-x(ios/android)开发环境

## 集成 JPush Cocos2d-x Android SDK



## 集成 JPush Cocos2d-x iOS SDK

* 使用cocos2d-x脚本生成ios工程,并打开该工程

* 添加必要的框架：
*
```
CoreTelehony.framework
```
```
Security.framework
```
```
CFNetwork.framework
```
```
CoreFoundation.framework
```
```
SystemConfiguration.framework
```
* 在工程中创建一个新的 Property List 文件，并将其命名为 PushConfig.plist，填入Portal 为你的应用提供的 APP_KEY 等参数

```
{
    "APS_FOR_PRODUCTION = "0";
    "CHANNEL" = "Publish channel";
    "APP_KEY" = "AppKey copied from JPush Portal application";
}
```

* 在Build Phases中的Link Binary With Libraries中加入libPushSDK.a静态库文件或者直接将libPushSDK.a拖入工程内。
  
*  找到 xcode 工程 Libraries 文件夹的 APServer.h，APServiceCpp.h,APServiceCpp.mm拖入 project 中(或者点击右键，点击 add files to "project name")

* 在 AppController.mm(注意不是AppDelegate.cpp) 中添加头文件APServer.h


```
#import "APServiceCpp.h"
```


* 在 AppController.mm 中添加监听系统事件，相应地调用 JPush SDK 提供的 API 来实现功能

```
  - (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
  {
  // Required
          ［APService registerForRemoteNotificationTypes:
           UIRemoteNotificationTypeBadge | UIRemoteNotificationTypeSound |
           UIRemoteNotificationTypeAlert］;
  // Required
          [APService setupWithOption:launchOptions];
          ......
          return YES;
 }
```
```
  	- (void)application:(UIApplication *)application 	didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
      		// Required
      		[APService registerDeviceToken:deviceToken];
  		}
```
```	
 	 - (void)application:(UIApplication *)application 	didReceiveRemoteNotification:(NSDictionary *)userInfo {
     	 // Required
     	 [APService handleRemoteNotification:userInfo];
 	 }
```
```
    //IOS7 only
	- (void)application:(UIApplication *)application
      didReceiveRemoteNotification:(NSDictionary *)userInfo
          fetchCompletionHandler:
              (void (^)(UIBackgroundFetchResult))completionHandler {
     [APService handleRemoteNotification:userInfo];
      completionHandler(UIBackgroundFetchResultNewData);
}
```
* 在需要处理推送回调的类中添加回调函数，相应地调用 JPush SDK 提供的 API 来实现功能  
 
```
JPushService::registerCallbackFunction(setupCallback, closeCallback,
                                         Register_callback, Login_callback,
                                         ReceiveMessage_callback);
```
* * 或者你也可以分别调用每一个回调函数的设置API方法

```
  static void registerSetupCallbackFunction(setupCallback);
  static void registerCloseCallbackFunction(closeCallback);
  static void registerRegisterCallbackFunction(Register_callback);
  static void registerLoginCallbackFunction(Login_callback);
  static void registerCallbackFunction(ReceiveMessage_callback);
```

* * API参数要符合头文件提供的函数指针

```
void setupCallback() { cout << "setup" << endl; }
```

* Tags、Alias设置方法

```
JPushService::setAliasAndTags("别名1", tags1, tagsAliasCallback);

```
* * tagsAliasCallback要符合头文件的函数指针

```
void tagsAliasCallback(int responseCode, const char *alias, c_tags tags) {
  set<string>::iterator it = tags->begin();
  for (; it != tags->end(); ++it) {
    const char *p = ((string)(*it)).c_str();
    cout << "tags:" << p << endl;
  }
  cout << "alias" << alias << endl;
  cout << "iResCode" << responseCode << endl;
}

```

* 获取RegistrationID

```
void Register_callback(const char *registrationID) {
  cout << "registration:" << registrationID << endl;
}
```

#### ##注意事项:在用filterValidTags函数之后，记得将返回的函数指针释放！
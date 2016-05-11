jpush-cocos2d-x-plugin
======================
[![Gitter](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/jpush/jpush-phonegap-plugin)
[![platforms](https://img.shields.io/badge/platforms-iOS%7CAndroid-lightgrey.svg)](https://github.com/jpush/jpush-cocos2d-x-plugin)
[![weibo](https://img.shields.io/badge/weibo-JPush-blue.svg)](http://weibo.com/jpush?refer_flag=1001030101_&is_all=1)

JPush's officially supported Cocos2d-x plugin (Android &amp; iOS). 极光推送官方支持的 Cocos2d-x 插件（Android &amp; iOS）。


## 集成 JPush Cocos2d-x iOS SDK
-----------------------
#### 1. 配置基本信息

* 使用 cocos2d-x 脚本生成 iOS 工程

* 添加必要框架。打开 xcode，点击 project，选择 (Targets -> Build Phases -> Link Binary With Libraries)

		CFNetwork.framework
		CoreFoundation.framework
		CoreTelephony.framework
		CoreGraphics.framework
		Foundation.framework
		UIKit.framework
		Security.framework
		libz.tbd//若原先有 libz.dylib 则替换为 libz.tbd
		AdSupport.framework//若需要使用 IDFA 广告标识符则添加该库

* 将 Plugins/JPushPlugin_iOS 文件夹及内容拖拽到 Xcode 工程里，拖拽时 Choose options for adding these files 选择：
	-  Destination：✓ Copy items if needed
	-  Added folders：✓ Create groups
	-  Add to targets：✓ your-cocos2d-x-proj
  
#### 2. 添加代码

* 	在 ios/AppController.mm (注意不是AppDelegate.cpp) 中添加头文件：

		#import "JPUSHService.h"
		//#import <AdSupport/AdSupport.h>//如需使用广告标识符 IDFA 则添加该头文件，否则不添加


* 在 AppController.mm 中添加以下代码：（如果方法存在，则只将其中代码添加至方法中；如果方法不存在，则添加方法及其中代码）

	```
	- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions{
		
		
		// Required
		#if __IPHONE_OS_VERSION_MAX_ALLOWED > __IPHONE_7_1
		    if ([[UIDevice currentDevice].systemVersion floatValue] >= 8.0) {
		        //可以添加自定义categories
		        [JPUSHService registerForRemoteNotificationTypes:(UIUserNotificationTypeBadge |
		                                                       UIUserNotificationTypeSound |
		                                                       UIUserNotificationTypeAlert)
		                                           categories:nil];
		        } else {
		            //categories 必须为nil
		            [JPUSHService registerForRemoteNotificationTypes:(UIRemoteNotificationTypeBadge |
		                                                           UIRemoteNotificationTypeSound |
		                                                           UIRemoteNotificationTypeAlert)
		                                               categories:nil];
		        }
		#else
		        //categories 必须为nil
		        [JPUSHService registerForRemoteNotificationTypes:(UIRemoteNotificationTypeBadge |
		                                                       UIRemoteNotificationTypeSound |
		                                                       UIRemoteNotificationTypeAlert)
		                                           categories:nil];
		#endif
		
		//启动 sdk
		/* （1）不使用 IDFA 启动 sdk
			参数说明：
            	appKey：极光官网控制台应用标识
            	channel：频道，暂无可填任意
            	apsForProduction：YES发布环境/NO开发环境
		*/
		[JPUSHService setupWithOption:launchOptions appKey:@"abcacdf406411fa656ee11c3" channel:@"" apsForProduction:NO];


		/* （2）使用 IDFA 启动 sdk （不与上述方法同时使用）
			参数说明：
				appKey：极光官网控制台应用标识
				channel：频道，暂无可填任意
				apsForProduction：YES发布环境/NO开发环境
				advertisingIdentifier：IDFA广告标识符
		*/
		//NSString *advertisingId = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
		//[JPUSHService setupWithOption:launchOptions appKey:@"abcacdf406411fa656ee11c3" channel:@"" apsForProduction:NO advertisingIdentifier:advertisingId];

    return YES;
		


    	......
	    return YES;
	}
	```
	```
	- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken{
		// Required
		[JPUSHService registerDeviceToken:deviceToken];
	}
	```
	```	
	- (void)application:(UIApplication *)application
	 		didReceiveRemoteNotification:(NSDictionary *)userInfo {
	  	// Required
	 	[JPUSHService registerDeviceToken:deviceToken];
	}
	```
	```
    //IOS7 only
	- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler {
  		[JPUSHService handleRemoteNotification:userInfo];
	  	completionHandler(UIBackgroundFetchResultNewData);
	}
	```
	
* 在需要处理推送回调的类中添加回调函数，相应地调用 JPush SDK 提供的 API 来实现功能,调用地方需要引入头文件JPushBridge.h

		#import "JPushBridge.h"

		JPushBridge::registerCallbackFunction(setupCallback, closeCallback,
                                         Register_callback, Login_callback,
                                         ReceiveMessage_callback);
                                         
                                         
* 或者你也可以分别调用每一个回调函数的设置API方法

		static void registerSetupCallbackFunction(setupCallback);
		static void registerCloseCallbackFunction(closeCallback);
		static void registerRegisterCallbackFunction(Register_callback);
		static void registerLoginCallbackFunction(Login_callback)
		static void registerCallbackFunction(ReceiveMessage_callback);

* API参数要符合头文件提供的函数指针

		void setupCallback() { cout << "setup" << endl; }

* Tags、Alias设置方法	,自定义tagsAliasCallback要符合头文件的函数指针

		JPushBridge::setAliasAndTags("别名1", tags1, tagsAliasCallback);
		
* Tags过滤方法：需要传入一个result指针用以获取过滤后的Tags. 

		bool filterValidTags(set<string> *tags, set<string> *result);

* 获取RegistrationID

		void register_callback(const char *registrationID)；
		

---------------------------------------------------------------------------
## 集成 JPush Cocos2d-x Android SDK


####执行脚本
* 将下载下来的`jpush-cocos2d-x-plugin`文件夹拖到`{COCOS2DX_ROOT}/plugin/plugins`目录下。
* 执行`jpush-cocos2d-x-plugin/Plugins/install_jpush.py`

		./install_jpush.py -project YourProjectName -package YourPackageName -appkey YourAppkey
	
	完成！

####使用API

JPush SDK 提供的 API 接口,都主要集中在 JPushService.h 类里。只需要在第一个游戏场景中：

- init 初始化SDK

		JPushService::init();
		
- setDebugMode 设置调试模式

		// You can enable debug mode in developing state. You should close debug mode when release.
	    JPushService::setDebugMode(true);
	   
就可以使用推送消息了。

#### 接收推送消息
这个动作不是必须的，如果不做这个动作，则默认的行为是：

- 接收到推送的自定义消息，则没有被处理
- 可以正常收到通知，用户点击打开应用主界面


**1.** 如果全部类型的广播都接收，则需要在 AndroidManifest.xml 里添加如下的配置信息：

	<receiver android:name="JPushReceiver" android:enabled="true">
	    <intent-filter>
	        <action android:name="cn.jpush.android.intent.REGISTRATION" />
	        <action android:name="cn.jpush.android.intent.MESSAGE_RECEIVED" />
	        <action android:name="cn.jpush.android.intent.NOTIFICATION_RECEIVED" />
	        <action android:name="cn.jpush.android.intent.NOTIFICATION_OPENED" />
	        <category android:name="You Package Name" />
	    </intent-filter>
	</receiver>
将`Your Package Name`替换成你自己的包名。

**2.** 将SDK中的`Android/JPushReceiver.java`放在`src`包名目录下.

**3.** 注册回调函数

首先定义一个回调函数，如：

	void handlerRemoteNotification(void* p_handler,const char *message){
		//当收到推送通知时，会触发这个回调函数，其中message参数是一个Json字符串，你可以
		//从中获取通知的详细信息
	}
然后调用
	   
	JPushService::registerRemoteNotifcationCallback(this, &handlerRemoteNotification);
	 
向JPushService注册此回调函数，具体字段可参考`JPushReceiver.java`类。

#### 测试确认
1. 确认所需的权限都已经添加。如果必须的权限未添加,日志会提示错误。
2. 确认 AppKey(在Portal上生成的)已经正确的写入 Androidmanifest.xml 。
3. 确认在程序启动时候调用了init() 接口
4. 确认测试手机(或者模拟器)已成功连入网络
客户端调用 init 后不久,如果一切正常,应有登录成功的日志信息
5. 启动应用程序,在 Portal 上向应用程序发送自定义消息或者通知栏提示。详情请参考管理Portal。
在几秒内,客户端应可收到下发的通知或者正定义消息.

#### 常见问题

##### multiple definition of 'getCallbackHelperObject

* 检查文件**jni/Android.mk**中**LOCAL_SRC_FILES :**是否重复包含**JPushService.cpp**

#####如何升级cocos2d-x plugin for android插件		
* 将`{COCOS2DX_ROOT}/plugin/plugins/jpush-cocos2d-x-plugin`文件夹删除，再按照上面的集成文档执行install_jpush.py脚本即可		

##### c++接口的怎么调用？
* c++的函数名称与java方法相对应，具体请参照[JPush文档: android的API](http://docs.jpush.cn/display/dev/API%3A+Android)

##### 在android的工程中加了其他的SDK时，重新编译时，其他SDK的so文件消息怎么办？

引起的原因:是因为libs/armeabi目录大小发生较大的变化时，每次编译会更新这个目录，导致我们的第三方库被删除

解决方案：

1. 在[your_android_project]/jni/preduild/Android.mk文件中 加入
		
		include $(CLEAR_VARS)
		LOCAL_MODULE := your_module
		LOCAL_SRC_FILES := your_project.so
		include $(PREBUILT_SHARED_LIBRARY)

2. 在[your_android_project]/jni/Andorid.mk中找到`LOCAL_SHARED_LIBRARIES := jpush_so
`将其修改成：

		LOCAL_SHARED_LIBRARIES := jpush_so your_project_so



##高级功能 
请参考:

[android 标签与别名API](http://docs.jpush.cn/pages/viewpage.action?pageId=557241)
[android 接收推送消息](http://docs.jpush.cn/pages/viewpage.action?pageId=1343602)

[ios 标签与别名API](http://docs.jpush.cn/pages/viewpage.action?pageId=3309913)

[ios 接收推送消息](http://docs.jpush.cn/pages/viewpage.action?pageId=3310013)

##技术支持
邮件联系:<support@jpush.cn> 

技术QQ群:132992583



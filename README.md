# jpush-cocos2d-x-plugin

[![Gitter](https://badges.gitter.im/Join%20Chat.svg)](https://gitter.im/jpush/jpush-phonegap-plugin)
[![platforms](https://img.shields.io/badge/platforms-iOS%7CAndroid-lightgrey.svg)](https://github.com/jpush/jpush-cocos2d-x-plugin)
[![weibo](https://img.shields.io/badge/weibo-JPush-blue.svg)](http://weibo.com/jpush?refer_flag=1001030101_&is_all=1)

JPush's officially supported Cocos2d-x plugin (Android &amp; iOS). 极光推送官方支持的 Cocos2d-x 插件（Android &amp; iOS）。


## iOS Project 集成 JPush 插件

#### 1. 配置基本信息

* 使用 Cocos2d-x 生成 iOS 工程

* 添加必要框架。打开 Xcode，点击 project，选择 (Targets -> Build Phases -> Link Binary With Libraries)，添加以下框架：

		CFNetwork.framework
		CoreFoundation.framework
		CoreTelephony.framework
		CoreGraphics.framework
		Foundation.framework
		UIKit.framework
		Security.framework
		SystemConfiguration.framework
		libz.tbd//Xcode7 以下是 libz.dylib
		AdSupport.framework//若需要使用 IDFA 广告标识符则添加该库

* 将插件的 /iOS/JPushPlugin 文件夹及内容拖拽到 Xcode 工程里，拖拽时 Choose options for adding these files 选择：
	-  Destination：✓ Copy items if needed
	-  Added folders：✓ Create groups
	-  Add to targets：✓ your-proj-name
  
#### 2. 添加代码

* 	在工程的 /ios/AppController.mm (注意不是 AppDelegate.cpp) 中添加头文件：

		#import "JPUSHService.h"
		//#import <AdSupport/AdSupport.h>//如需使用广告标识符 IDFA 则添加该头文件，否则不添加


* 在 AppController.mm 中添加以下代码：（如果方法存在，则只将其中代码添加至方法中；如果方法不存在，则添加方法及其中代码）

	```
	- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions{
		
		
		// 注册推送
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
	- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo {
	  	// Required
		[JPUSHService handleRemoteNotification:userInfo];
	}
	```
	```
    //IOS7 only
	- (void)application:(UIApplication *)application didReceiveRemoteNotification:(NSDictionary *)userInfo fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler {
  		[JPUSHService handleRemoteNotification:userInfo];
	  	completionHandler(UIBackgroundFetchResultNewData);
	}
	```
	
* 在需要处理推送回调的类中添加回调函数，相应地调用 JPush SDK 提供的 API 来实现功能,调用地方需要引入头文件 JPushBridge.h

		#import "JPushBridge.h"

		JPushBridge::registerCallbackFunction(setupCallback, closeCallback,
                                         Register_callback, Login_callback,
                                         ReceiveMessage_callback);
                                         
                                         
* 或者你也可以分别调用每一个回调函数的设置 API 方法

		static void registerSetupCallbackFunction(setupCallback);
		static void registerCloseCallbackFunction(closeCallback);
		static void registerRegisterCallbackFunction(Register_callback);
		static void registerLoginCallbackFunction(Login_callback)
		static void registerCallbackFunction(ReceiveMessage_callback);

* API 参数要符合头文件提供的函数指针

		void setupCallback() { cout << "setup" << endl; }

* Tags、Alias 设置方法,自定义 tagsAliasCallback 要符合头文件的函数指针

		JPushBridge::setAliasAndTags("别名1", tags1, tagsAliasCallback);
		
* Tags 过滤方法：需要传入一个 result 指针用以获取过滤后的 Tags. 

		bool filterValidTags(set<string> *tags, set<string> *result);

* 获取 RegistrationID

		void register_callback(const char *registrationID)；
		


## Android & Android Studio Project 集成 JPush 集成


#### 执行自动安装脚本

- 将工程文件 `YourProjectName` 置于 cocos2d-x-3.x `/projects/` 目录下

- 将插件 `jpush-cocos2d-x-plugin` 文件夹置于 ocos2d-x-3.x `/plugin/plugins/` 目录下。

- 使用命令行工具，进入插件目录 `/jpush-cocos2d-x-plugin/Android/`执行自动安装脚本
	- Android 工程（pro.android）执行 install_android.py
	
			./install_android.py -project YourProjectName -package YourPackageName -appkey YourAppkey
		

	- Android Studio 工程（proj.android-studio）执行 install_android_studio.py
	
			./install_android_studio.py -project YourProjectName -package YourPackageName -appkey YourAppkey

	
	显示 `JPush SDK installed successfully,have fun!` 则安装成功！
	
- 因目录结构不同可能导致安装失败，关键位置目录关系参照如下(以 cocos2d-x-3.10 为例)：
		
			../Cocos2d-x/cocos2d-x-3.10/
			├─┬ /plugin/plugins/jpush-cocos2d-x-plugin/Android/
			│ ├── install_android.py
			│ └── install_android_studio.py
			└─┬ /projects/YourProjectName/
			  ├── /Classes/
			  ├─┬ /proj.android/
			  │ ├── /jni/
			  │ ├── /libs/
			  │ └── /src/
			  └─┬ /proj.android-studio/app/
			    ├── /jni/
			    ├── /libs/
			    └── /src/
	- 因 Cocos2d-x 版本不同导致目录变化，可相应的对自己的目录进行调整，以便成功安装
	- 自动安装失败时可以尝试 [手动安装](https://github.com/jpush/jpush-cocos2d-x-plugin/issues/1)
			    
#### 使用 API

JPush SDK 提供的 API 接口,都主要集中在 JPushBridge.h 类里。只需要在第一个游戏场景中：

- init 初始化 SDK

		JPushBridge::init();
		
- setDebugMode 设置调试模式

		// You can enable debug mode in developing state. You should close debug mode when release.
	    JPushBridge::setDebugMode(true);
	   
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
将 `Your Package Name` 替换成你自己的包名。

**2.** 将 SDK 中的 `Android/JPushReceiver.java` 放在 `src` 包名目录下.

**3.** 注册回调函数

首先定义一个回调函数，如：

	void handlerRemoteNotification(void* p_handler,const char *message){
		//当收到推送通知时，会触发这个回调函数，其中message参数是一个Json字符串，你可以
		//从中获取通知的详细信息
	}
然后调用
	   
	JPushBridge::registerRemoteNotifcationCallback(this, &handlerRemoteNotification);
	 
向 JPushBridge 注册此回调函数，具体字段可参考 `JPushReceiver.java` 类。

#### 测试确认
1. 确认所需的权限都已经添加。如果必须的权限未添加,日志会提示错误。
2. 确认 AppKey (在 Portal 上生成的)已经正确的写入 Androidmanifest.xml 。
3. 确认在程序启动时候调用了 init() 接口
4. 确认测试手机(或者模拟器)已成功连入网络
客户端调用 init 后不久,如果一切正常,应有登录成功的日志信息
5. 启动应用程序,在 Portal 上向应用程序发送自定义消息或者通知栏提示。详情请参考管理 Portal。
在几秒内,客户端应可收到下发的通知或者正定义消息.

#### 常见问题

##### multiple definition of 'getCallbackHelperObject

* 检查文件 **jni/Android.mk** 中 **LOCAL_SRC_FILES :** 是否重复包含 **JPushBridge.cpp**

#####如何升级 Cocos2d-x plugin for Android 插件		
* 将 `{COCOS2DX_ROOT}/plugin/plugins/jpush-cocos2d-x-plugin` 文件夹删除，再按照上面的集成文档执行install_jpush.py脚本即可		

##### C++ 接口的怎么调用？
* C++ 的函数名称与 java 方法相对应，具体请参照[JPush文档: Android 的 API](http://docs.jpush.cn/display/dev/API%3A+Android)

##### 在 Android 的工程中加了其他的 SDK 时，重新编译时，其他 SDK 的 so 文件消息怎么办？

引起的原因:是因为 libs/armeabi 目录大小发生较大的变化时，每次编译会更新这个目录，导致我们的第三方库被删除

解决方案：

1. 在 [your_android_project]/jni/preduild/Android.mk 文件中 加入
		
		include $(CLEAR_VARS)
		LOCAL_MODULE := your_module
		LOCAL_SRC_FILES := your_project.so
		include $(PREBUILT_SHARED_LIBRARY)

2. 在 [your_android_project]/jni/Andorid.mk 中找到 `LOCAL_SHARED_LIBRARIES := jpush_so
` 将其修改成：

		LOCAL_SHARED_LIBRARIES := jpush_so your_project_so



## 高级功能 
请参考:

[Android 标签与别名 API](http://docs.jpush.cn/pages/viewpage.action?pageId=557241)
[Android 接收推送消息](http://docs.jpush.cn/pages/viewpage.action?pageId=1343602)

[iOS 标签与别名 API](http://docs.jpush.cn/pages/viewpage.action?pageId=3309913)

[iOS 接收推送消息](http://docs.jpush.cn/pages/viewpage.action?pageId=3310013)

## 技术支持
邮件联系: <support@jpush.cn> 

极光社区(答疑论坛): [http://community.jpush.cn/](http://community.jpush.cn/)



jpush-cocos2d-x-plugin
======================

JPush's officially supported Cocos2d-x plugin (Android &amp; iOS). 极光推送官方支持的 Cocos2d-x 插件（Android &amp; iOS）。

### 导入到 Cocos2d-x 项目

- 搭建好Cocos2d-x(iOS/Android)开发环境


## 集成 JPush Cocos2d-x iOS SDK
-----------------------
#### 1. 配置基本信息

* 使用cocos2d-x脚本生成iOS工程,并打开该工程

* 添加必要的框架
`CoreTelephony.framework`,
`Security.framework`,
`CFNetwork.framework`,
`CoreFoundation.framework`,
`SystemConfiguration.framework`。

* 	创建并配置PushConfig.plist文件，在你的工程中创建一个新的Property List文件，并将其命名为PushConfig.plist，填入Portal为你的应用提供的APP_KEY等参数。

	CHANNEL指明应用程序包的下载渠道，为方便分渠道统计。根据你的需求自行定义即可。APP_KEY在管理Portal上创建应用时自动生成的（AppKey）用以标识该应用。请确保应用内配置的 AppKey 与第1步在 Portal 上创建应用时生成的 AppKey 一致，AppKey 可以在应用详情中查询。

	APS_FOR_PRODUCTION表示应用是否采用生产证书发布( Ad_Hoc 或 APP Store )，0 (默认值)表示采用的是开发者证书，1 表示采用生产证书发布应用。请注意此处配置与 Web Portal 应用环境设置匹配。

	
		{
	    "APS_FOR_PRODUCTION = "0";
    	"CHANNEL" = "Publish channel";
	    "APP_KEY" = "AppKey copied from JPush Portal application";
		}

*	Build Settings设置 Search Paths 下的 User Header Search Paths 和 Library Search Paths，比如SDK文件夹（默认为lib）与工程文件在同一级目录下，则都设置为"$(SRCROOT)/[文件夹名称]"即可。

*	在XCode中选择“Add files to 'Your project name'...”，将lib子文件夹中的libPushSDK.a添加到你的工程目录中。
  
*	将Plugins/iOS/lib 文件夹下的 APServer.h，APServiceCpp.h,APServiceCpp.mm拖入 project 中(或者点击右键，点击 add files to "project name")，APServer.h拖入Class文件夹中,和安卓共享同一个，APServiceCpp.h,APServiceCpp.mm拖入ios文件夹下.

#### 2. 添加代码

* 	在ios/AppController.mm(注意不是AppDelegate.cpp) 中添加头文件`APService.h`

		#import "APService.h"

* 在 AppController.mm 中添加监听系统事件，相应地调用 JPush SDK 提供的 API 来实现功能

	```
	- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions{
		
		
		// Required
		[APService registerForRemoteNotificationTypes:UIRemoteNotificationTypeBadge |
									UIRemoteNotificationTypeSound | 
									UIRemoteNotificationTypeAlert];
		// Required
     	[APService setupWithOption:launchOptions];
    	......
	    return YES;
	}
	```
	```
	- (void)application:(UIApplication *)application 
	didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken{
		// Required
		[APService registerDeviceToken:deviceToken];
	}
	```
	```	
	- (void)application:(UIApplication *)application
	 		didReceiveRemoteNotification:(NSDictionary *)userInfo {
	  	// Required
	 	[APService registerDeviceToken:deviceToken];
	}
	```
	```
    //IOS7 only
	- (void)application:(UIApplication *)application
	didReceiveRemoteNotification:(NSDictionary *)userInfo
		fetchCompletionHandler:(void (^)(UIBackgroundFetchResult))completionHandler {
  		[APService handleRemoteNotification:userInfo];
	  	completionHandler(UIBackgroundFetchResultNewData);
	}
	```
* 在需要处理推送回调的类中添加回调函数，相应地调用 JPush SDK 提供的 API 来实现功能,调用地方需要引入头文件JPushService.h

		#import "JPushService.h"

		JPushService::registerCallbackFunction(setupCallback, closeCallback,
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

		JPushService::setAliasAndTags("别名1", tags1, tagsAliasCallback);
		
* Tags过滤方法：需要传入一个result指针用以获取过滤后的Tags. 

		bool filterValidTags(set<string> *tags, set<string> *result);

* 获取RegistrationID

		void register_callback(const char *registrationID)；
		

---------------------------------------------------------------------------
## 集成 JPush Cocos2d-x Android SDK



####1. 在项目中导入SDK开发包
- 复制 `libs/jpush-sdk-release1.6.1.jar` 到工程__libs/__目录下- 复制 `prebuild` 文件夹到jni目录下

刷新此目录。修改jni目录下的__Android.mk__,添加：

		include $(LOCAL_PATH)/prebuild/Android.mk
		LOCAL_SHARED_LIBRARIES := jpush_so

- 将Plugins中的`/JPushService.h`与Android目录下的`Android/JPushService.cpp`、拖入到工程中合适的位置，（`JPushService.h`放Classes目录下，`JPushService.cpp`放在jni目录下）并修改Android.mk文件中的`LOCAL_SRC_FILES`确保`JPushService.cpp`能被顺利编译。
- 修改`LOCAL_C_INCLUDES`确保`JPushService.h`能被其他源文件正确引用，并将`$(LOCAL_PATH)/../../../../cocos2dx/platform/android/jni
`加入到`LOCAL_C_INCLUDES`之中确保`JniHelper.h`能被找到。
-  将`Android/JPushCallbackHelper.java`放在`src`包名目录下。

####2. 配置 AndroidManifest.xml**3.1 使用脚本自动配置**

运行manifest_util.py
	
	python manifest_util.py

然后根据提示输入android项目下`manifest.xml`的路径，以及对应的Portal上注册的`app key`.显示done之后配置成功。 根据需要选择是否添加下列权限

	   <!-- Optional. Required for location feature -->
    <uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION" />
    <uses-permission android:name="android.permission.ACCESS_COARSE_UPDATES" />
    <uses-permission android:name="android.permission.ACCESS_WIFI_STATE" />
    <uses-permission android:name="android.permission.CHANGE_WIFI_STATE" />
    <uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" />
    <uses-permission android:name="android.permission.ACCESS_LOCATION_EXTRA_COMMANDS" />
    <uses-permission android:name="android.permission.CHANGE_NETWORK_STATE" />

**3.2手动配置**
	如果不使用自动配置，可手动配置manifest。主要步骤为:	
1. 复制备注为 "Required" 的部分2. 将备注为替换包名的部分,替换为当前应用程序的包名3. 将AppKey替换为在Portal上注册该应用的的Key,例如(9fed5bcb7b9b87413678c407)

		<!-- Required -->
		<permission android:name="Your Package Name.permission.JPUSH_MESSAGE" 
        android:protectionLevel="signature" />
        <!-- End -->
        
        <application android:label="@string/app_name"
        android:icon="@drawable/icon" >
        
        <activity android:name=".Your Activity Name"
                  android:label="@string/app_name"
                  android:screenOrientation="landscape"
                  android:theme="@android:style/Theme.NoTitleBar.Fullscreen"
                  android:configChanges="orientation">
                  <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
       
       <!-- Required -->
       
		<service
		    android:name="cn.jpush.android.service.PushService"
		    android:enabled="true"
		    android:exported="false" >
		    <intent-filter>
		        <action android:name="cn.jpush.android.intent.REGISTER" />
		        <action android:name="cn.jpush.android.intent.REPORT" />
		        <action android:name="cn.jpush.android.intent.PushService" />
		    </intent-filter>
		</service>
		
		<receiver
			    android:name="cn.jpush.android.service.PushReceiver"
			    android:enabled="true" >
		    <intent-filter >
		        <action android:name="cn.jpush.android.intent.NOTIFICATION_RECEIVED_PROXY" />
		        <category android:name="Your Package Name" />
		    </intent-filter>>
			<intent-filter>
			    <action android:name="android.intent.action.USER_PRESENT" />
				<action	android:name="android.net.conn.CONNECTIVITY_CHANGE" />
			</intent-filter>
			
			<intent-filter>
			    <action android:name="android.intent.action.PACKAGE_ADDED" />
				<action android:name="android.intent.action.PACKAGE_REMOVED" />
			    <data android:scheme="package" />
			</intent-filter>
		</receiver>
		
		<activity
				android:name="cn.jpush.android.ui.PushActivity"
				android:theme="@android:style/Theme.Translucent.NoTitleBar"
				android:configChanges="orientation|keyboardHidden" >
		 	<intent-filter>
    			<action android:name="cn.jpush.android.ui.PushActivity" />
    			<category android:name="android.intent.category.DEFAULT" />
    			<category android:name="Your Package Name" />
			</intent-filter>
		</activity>
		
		<service
    		android:name="cn.jpush.android.service.DownloadService"
    		android:enabled="true"
    		android:exported="false" >
		</service>
		
		<receiver android:name="cn.jpush.android.service.AlarmReceiver" />
		
		<!-- Required. For publish channel feature -->
        <meta-data android:name="JPUSH_CHANNEL"
        android:value="developer-default"/>
        <!-- Required. AppKey copied from Portal -->
        <meta-data android:name="JPUSH_APPKEY" android:value="Your App Key"/>	
        <!-- END -->
        </application>
    	
    	<!-- Required -->
    	<uses-permission android:name="android.permission.INTERNET"/>
    	<uses-permission android:name="Your Package Name.permission.JPUSH_MESSAGE" />
	    <uses-permission android:name="android.permission.RECEIVE_BOOT_COMPLETED" />
	    <uses-permission android:name="android.permission.RECEIVE_USER_PRESENT" />
	    <uses-permission android:name="android.permission.INTERNET" />
	    <uses-permission android:name="android.permission.WAKE_LOCK" />
    	<uses-permission android:name="android.permission.READ_PHONE_STATE" />
	    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
	    <uses-permission android:name="android.permission.WRITE_SETTINGS" />
    	<uses-permission android:name="android.permission.READ_EXTERNAL_STORAGE" />
    	<uses-permission android:name="android.permission.VIBRATE" />
    	<uses-permission android:name="android.permission.MOUNT_UNMOUNT_FILESYSTEMS" />
	    <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
	    <!-- END -->

    	<!-- Optional. Required for location feature -->
	    <uses-permission android:name="android.permission.ACCESS_COARSE_LOCATION" />
    	<uses-permission android:name="android.permission.ACCESS_COARSE_UPDATES" />
	    <uses-permission android:name="android.permission.ACCESS_WIFI_STATE" />
    	<uses-permission android:name="android.permission.CHANGE_WIFI_STATE" />
    	<uses-permission android:name="android.permission.ACCESS_FINE_LOCATION" />
	    <uses-permission android:name="android.permission.ACCESS_LOCATION_EXTRA_COMMANDS" />
    	<uses-permission android:name="android.permission.CHANGE_NETWORK_STATE" />
    	<!-- Optional. Required for location feature END	 -->
    	
####4. 添加代码
添加获取Context代码,在游戏主Activity中加入如下代码：

	public static Context STATIC_REF = null;
	public static Context getContext(){
        return STATIC_REF;
    }
    
在`onCreate()`函数中添加获取context代码，如：

    protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);	
		STATIC_REF = this.getApplicationContext(); //request!
	}

修改JpushService.cpp

	//修改为包名+主Activity的名字，如com/JPush/Excample/MainActivity
		const char* kActivityName = "Your Main Activity"; 
	
	//将Yout Package Name 替换成你自己的包名如com/JPush/Excample
		const char* kCallbackClassName = "Your Package Name/JPushCallbackHelper";


	将方法中得Your Package Name替换成你自己的包名，如_com_JPush_Excample_

####5. 终于可以用了
JPush SDK 提供的 API 接口,都主要集中在 JPushService.h 类里。只需要在第一个游戏场景中：

- init 初始化SDK

		JPushService::init();
		
- setDebugMode 设置调试模式

		// You can enable debug mode in developing state. You should close debug mode when release.
	    JPushService::setDebugMode(true);
	   
就可以使用推送消息了。

####6. 接收推送消息
这个动作不是必须的，如果不做这个动作，则默认的行为是：

- 接收到推送的自定义消息，则没有被处理
- 可以正常收到通知，用户点击打开应用主界面


**6.1** 如果全部类型的广播都接收，则需要在 AndroidManifest.xml 里添加如下的配置信息：

	<receiver
	    android:name="JPushReceiver"
	    android:enabled="true">
	    <intent-filter>
	        <action android:name="cn.jpush.android.intent.REGISTRATION" />
	        <action android:name="cn.jpush.android.intent.MESSAGE_RECEIVED" />
	        <action android:name="cn.jpush.android.intent.NOTIFICATION_RECEIVED" />
	        <action android:name="cn.jpush.android.intent.NOTIFICATION_OPENED" />
	        
	        <category android:name="You package Name" />
	    </intent-filter>
	</receiver>
将`Your Package Name`替换成你自己的包名。

**6.2** 将SDK中的`Android/JPushReceiver.java`放在`src`包名目录下.

**6.3** 注册回调函数

首先注册一个回调函数，如：

	void handlerRemoteNotification(void* p_handler,const char *message){
		//当收到推送通知时，会触发这个回调函数，其中message参数是一个Json字符串，你可以
		//从中获取通知的详细信息
	}
然后调用
	   
	JPushService::registerRemoteNotifcationCallback(this, &handlerRemoteNotification);
	 
向JPushService注册此回调函数，具体字段可参考`JPushReceiver.java`类。

####7. 测试确认1. 确认所需的权限都已经添加。如果必须的权限未添加,日志会提示错误。2. 确认 AppKey(在Portal上生成的)已经正确的写入 Androidmanifest.xml 。
3. 确认在程序启动时候调用了init() 接口4. 确认测试手机(或者模拟器)已成功连入网络客户端调用 init 后不久,如果一切正常,应有登录成功的日志信息5. 启动应用程序,在 Portal 上向应用程序发送自定义消息或者通知栏提示。详情请参考管理Portal。在几秒内,客户端应可收到下发的通知或者正定义消息.
高级功能 请参考:[标签与别名API]()
[接收推送消息]()
技术支持
邮件联系: support@jpush.cn 
技术QQ群:132992583





jpush-cocos2d-x-plugin
======================

JPush's officially supported Cocos2d-x plugin (Android &amp; iOS). 极光推送官方支持的 Cocos2d-x 插件（Android &amp; iOS）。

## 导入到 Cocos2d-x 项目

* 搭建好Cocos2d-x(ios/android)开发环境


## 集成 JPush Cocos2d-x iOS SDK

* 使用cocos2d-x脚本生成ios工程,并打开该工程

* 添加必要的框架
```
CoreTelehony.framework
Security.framework
CFNetwork.framework
CoreFoundation.framework
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

---------------------------------------------------------------------------

## 集成 JPush Cocos2d-x Android SDK


####1. 编译项目
- 确保正确引用`Classes`、`cocos2dx`、`extensions`、`scripting` 四个文件夹（可通过右键点击文件夹，点击Properties，手动修改Resource下的Location引用）。
- 确保正确添加`NDK_ROOT`环境变量，（可通过右键点击项目，点击Properties，手动在C/C++ Build目录中的Environment中添加）。
- 编译项目，第一次编译目录中会增加 __libs__ 文件夹。

####2. 在项目中导入SDK开发包
- 复制 `libs/jpush-sdk-release.jar` 到工程__libs/__目录下
- 复制 `prebuild` 文件夹到jni目录下

刷新此目录。修改jni目录下的__Android.mk__,添加：

		include $(LOCAL_PATH)/prebuild/Android.mk
		LOCAL_SHARED_LIBRARIES := jpush_so

- 将`JPushService.h`、`JPushService.cpp`、`JPushUtil.h` 、`JPushUtil.cpp` 拖入到工程中合适的位置，（`JPushService.h`放在C++环境下，其他三个文件放在JNI目录下）并修改你的工程makefile文件的`LOCAL_SRC_FILES`确保`JPushInterface.cpp`与`JPushUtil.cpp`能被顺利编译。修改`LOCAL_C_INCLUDES`确保`JPushService.h`能被其他源文件正确引用，并将`[COCOS2DX_ROOT]/cocos2dx/platform/android/jni`加入到`LOCAL_C_INCLUDES`之中确保`JniHelper.h`能被找到（`[COCOS2DX_ROOT]`修改为cocos2dx库的目录）。
-  将`JPushCallbackHelper.java`放在`src`目录下。

####3. 配置 AndroidManifest.xml


**3.1 使用脚本自动配置**

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
	
根据 SDK 压缩包里的 AndroidManifest.xml 样例文件,来配置应用程序项目的 AndroidManifest.xml 。
主要步骤为:	

1. 复制备注为 "Required" 的部分
2. 将备注为替换包名的部分,替换为当前应用程序的包名
3. 将AppKey替换为在Portal上注册该应用的的Key,例如(9fed5bcb7b9b87413678c407)

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

```
	public static Context STATIC_REF = null;
```

```	
	@Override
	public void onCreate(Bundle savedInstanceState) {
    	super.onCreate(savedInstanceState);
    	STATIC_REF = getApplicationContext();
    //你的其他初始化代码
	}
```

修改JpushService.cpp

	//修改为主Activity的名字，如com/JPush/mainActivity
	const char* kActivityName = "Your Main Activity"; 
	//将Yout Package Name 替换成你自己的包名如com/JPush/Excample
	const char* kCallbackClassName = "Your Package Name/JPushCallbackHelper";

修改jni 目录下的`main.cpp`,正确引用`JPushUtil.h`、`JPushInterface.h
`,并添加以下方法

	JNIEXPORT void JNICALL Java_Your Package Name_setAliasAndTagsCallback(JNIEnv *env,jclass,jlong func_handler,jint resultCode, jstring alias,jobject tags,jlong func_ptr){
		int result = resultCode;
		const char *c_alias;
		if(alias!=NULL){
			c_alias = env->GetStringUTFChars(alias, JNI_FALSE);
		}
		std::set<std::string> *c_tags = JPushUtil::getStdStringSet(tags);
		long callback_ptr = func_ptr;
		long callback_handler = func_handler;
		APTagAliasCallback callback = (APTagAliasCallback)callback_ptr;
		void * p_handler = (void *)callback_handler;
		callback(p_handler,result,c_alias,c_tags);
		if(alias!=NULL){
			env->ReleaseStringUTFChars(alias, c_alias);
		}
	}
	
将Your Package Name替换成你自己的包名，如__com_Jpush_Excample__

####5. 终于可以用了
JPush SDK 提供的 API 接口,都主要集中在 JpushService.h 类里。只需要在第一个游戏场景中：

- init 初始化SDK

		JPushService::init();
		
- setDebugMode 设置调试模式

		// You can enable debug mode in developing state. You should close debug mode when release.
	    static void setDebugMode(bool enable)
	   
就可以使用推送消息了。
	    
####6. 测试确认
1. 确认所需的权限都已经添加。如果必须的权限未添加,日志会提示错误。
2. 确认 AppKey(在Portal上生成的)已经正确的写入 Androidmanifest.xml 。
3. 确认在程序启动时候调用了init(context) 接口
4. 确认测试手机(或者模拟器)已成功连入网络
客户端调用 init 后不久,如果一切正常,应有登录成功的日志信息
5. 启动应用程序,在 Portal 上向应用程序发送自定义消息或者通知栏提示。详情请参考管理Portal。
在几秒内,客户端应可收到下发的通知或者正定义消息.


高级功能 请参考:
[标签与别名API]()
[接收推送消息]()

技术支持


邮件联系: support@jpush.cn 


技术QQ群:132992583

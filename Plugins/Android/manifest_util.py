
import xml.etree.ElementTree as ET
import sys
import re

data = """<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="Your_Package_Name"
    android:versionCode="1"
    android:versionName="1.0">
    
    <permission android:name="Your_Package_Name.permission.JPUSH_MESSAGE"
    android:protectionLevel="signature" />
    
    <application android:label="@string/app_name"
        android:icon="@drawable/icon">
        
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
		<!-- Required -->
		<receiver
            android:name="cn.jpush.android.service.PushReceiver"
            android:enabled="true" >
		    <intent-filter >
		        <action android:name="cn.jpush.android.intent.NOTIFICATION_RECEIVED_PROXY" />
		        <category android:name="Your_Package_Name" />
		    </intent-filter>
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
    			<category android:name="Your_Package_Name" />
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
        <meta-data android:name="JPUSH_APPKEY" android:value="app_key"/>
    </application>

    
    <uses-permission android:name="android.permission.INTERNET"/>
    <uses-permission android:name="Your_Package_Name.permission.JPUSH_MESSAGE" />
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
 
</manifest>"""

ET.register_namespace("android","http://schemas.android.com/apk/res/android")

manifest = raw_input("Please input the file path of your Manifest.xml: ")
appKey = raw_input("Please input the key you registed on JPush Portal: ")

tree_one = ET.parse(manifest)
first_root = tree_one.getroot()
package_name = first_root.attrib['package']

data = re.sub('Your_Package_Name',package_name,data,flags=re.I)
data =re.sub('app_key',appKey,data,flags=re.I)

tree_two = ET.ElementTree(ET.fromstring(data))
second_root = tree_two.getroot()

permission = second_root.findall('permission')
first_root.extend(permission)

ele = second_root.find('application')
app_ele = first_root.find('application')
app_ele.extend(ele)

permission = second_root.findall('uses-permission')
first_root.extend(permission)


#for element in first_root.iter():
#   print element.tag,element.attrib

tree = ET.ElementTree(first_root)
tree.write(manifest,"utf-8",xml_declaration=True)
print ("done")

#!/usr/bin/python
#install_jpush.py

context = {
"src_project_name"  : "undefined",
"src_package_name"  : "undefined",
"dst_project_name"  : "undefined",
"dst_package_name"  : "undefined",
"src_project_path"  : "undefined",
"dst_project_path"  : "undefined",
"script_dir"        : "undefined",
"appkey"            : "undefined",
}

import sys
import os, os.path
import json
import shutil
import xml.etree.ElementTree as ET
import sys
import re
import glob

def dumpUsage():
    print "Usage: install_jpush.py -project PROJECT_NAME -package PACKAGE_NAME -appkey APP_KEY"
    print "Options:"
    print "  -project    Project name, for example: MyGame"
    print "  -package    Package name, for example: com.MyCompany.MyAwesomeGame "
    print "  -appkey     APP key for JPush portal "
    print "Sample 1: ./install_jpush.py -project pluginTest -package com.MyCompany.AwesomeGame  -appkey 997f28c1cea5a9f17d82079a "

# end of dumpUsage(context) function


def checkParams(context):
    # generate our internal params
    context["script_dir"] = os.getcwd() + "/"
    global platforms_list
    
    # invalid invoke, tell users how to input params
    if len(sys.argv) < 7:
        dumpUsage()
        sys.exit()
    
    # find our params
    for i in range(1, len(sys.argv)):
        if "-project" == sys.argv[i]:
            # read the next param as project_name
            context["dst_project_name"] = sys.argv[i+1]
            context["dst_project_path"] = os.getcwd() + "/../../../../projects/"
        elif "-package" == sys.argv[i]:
            # read the next param as g_PackageName
            context["dst_package_name"] = sys.argv[i+1]
        elif "-appkey" == sys.argv[i]:
            # choose a scripting language
            context["appkey"] = sys.argv[i+1]
    
    # pinrt error log our required paramters are not ready
    raise_error = False
    if context["dst_project_name"] == "undefined":
        print "Invalid -project parameter"
        raise_error = True
    if context["dst_package_name"] == "undefined":
        print "Invalid -package parameter"
        raise_error = True
    if context["appkey"] == "undefined":
        print "Invalid -appkey parameter"
        raise_error = True
    if raise_error != False:
        sys.exit()
    # fill in src_project_name and src_package_name according to "language"
    context["src_project_name"] = "HelloCpp"
    context["src_package_name"] = "org.cocos2dx.hellocpp"
    context["src_project_path"] = os.getcwd() + "/Android/"
# end of checkParams(context) function
def copyToProjcect():
    #print "copying To Project"
    libSource=context["src_project_path"]+"libs/jpush-sdk-release1.6.4.jar"
    libTarget=context["dst_project_path"]+context["dst_project_name"]+"/proj.android/libs/"
    if not os.path.exists(libTarget):
        os.makedirs(libTarget)
    else:
        for f in glob.glob(libTarget+"/jpush-sdk-release*.jar"):
            os.remove(f)
        for directory in glob.glob(libTarget+"/armeabi*/"):
            shutil.rmtree(directory)
    shutil.copy(libSource,libTarget)
    #print "copy jpush libs done!"
    prebuildSource=context["src_project_path"]+"libs/prebuild/"
    prebuildTarget=context["dst_project_path"]+context["dst_project_name"]+"/proj.android/jni/prebuild/"
    if os.path.exists(prebuildTarget):
        shutil.rmtree(prebuildTarget)
    shutil.copytree(prebuildSource,prebuildTarget)

    soSource=context["src_project_path"]+"libs/armeabi"
    v7aSoSource=context["src_project_path"]+"libs/armeabi-v7a"

    soTarget=context["dst_project_path"]+context["dst_project_name"]+"/proj.android/libs/armeabi"
    v7aSoTarget=context["dst_project_path"]+context["dst_project_name"]+"/proj.android/libs/armeabi-v7a"

    shutil.copytree(soSource,soTarget)
    shutil.copytree(v7aSoSource,v7aSoTarget)

    #print "copy prebuild done!"
#libs/jpush-sdk-release1.6.1.jar to libs
# end of copyToProjcect(context) function
def copyTextToAndroidmk():
    #print "copying Text to Android.mk"
    mkTargetPath=context["dst_project_path"]+context["dst_project_name"]+"/proj.android/jni/Android.mk"
    fp=file(mkTargetPath)
    lines=[]
    for line in fp: 
        lines.append(line)
    fp.close()
    i=0
    startRepeat=-1;
    endRepeat=-1;
    for repeatContent in lines:
        if repeatContent.find("include $(LOCAL_PATH)/prebuild/Android.mk"):
            startRepeat=i
        if repeatContent.find("                 JPushService.cpp \\"):
            endRepeat=i
        i+=1
    if startRepeat>0 and endRepeat >0 and endRepeat > startRepeat:
        del lines[startRepeat:endRepeat+1]
    i=0
    for content in lines:
        i=i+1
        if content.find("include $(BUILD_SHARED_LIBRARY)") >= 0:
            break
    lines.insert(i, "include $(LOCAL_PATH)/prebuild/Android.mk")
    i=i+1
    lines.insert(i,"\n")
    i=i+1
    lines.insert(i, "LOCAL_SHARED_LIBRARIES := jpush_so");
    i=i+1
    lines.insert(i,"\n")
    i=0
    for linesub in lines:
        i=i+1
        if linesub.find("hellocpp/main.cpp")>=0:
            break
    lines.insert(i,"					JPushService.cpp \\");
    i=i+1;
    lines.insert(i,"\n");
    fp=file(mkTargetPath,'w')
    for s in lines:   
        fp.write(s)
    fp.close()
    #print "copy to Android.mk done"
# end of copyTextToAndroidmk(context) function
def copyCodeToProject():
    #print "copying code to Project"
    hfileSource = context["src_project_path"]+"../JPushService.h"
    hfileTarget = context["dst_project_path"]+context["dst_project_name"]+"/Classes/JPushService.h"
    hfileTargetPath = context["dst_project_path"]+context["dst_project_name"]+"/Classes/"
    if not os.path.exists(hfileTargetPath):
        os.makedirs(hfileTargetPath)
    elif os.path.exists(hfileTarget):
        os.remove(hfileTarget)
    shutil.copy(hfileSource,hfileTarget)

    cppfileSource = context["src_project_path"]+"JPushService.cpp"
    cppfileTarget = context["dst_project_path"]+context["dst_project_name"]+"/proj.android/jni/JPushService.cpp"
    cppfileTargetPath=context["dst_project_path"]+context["dst_project_name"]+"/proj.android/jni/"
    if not os.path.exists(cppfileTargetPath):
        os.makedirs(cppfileTargetPath)
    elif os.path.exists(cppfileTarget):
        os.remove(cppfileTarget)
    shutil.copy(cppfileSource,cppfileTarget)

    javafileSource = context["src_project_path"]+"JPushCallbackHelper.java"
    javafileTarget = context["dst_project_path"]+context["dst_project_name"]+"/proj.android/src/"
    paths=context["dst_package_name"].split('.');
    for path in paths:
        javafileTarget=javafileTarget+path+"/"
    javafileTarget=javafileTarget+"JPushCallbackHelper.java"
    if os.path.exists(javafileTarget):
        os.remove(javafileTarget)
    shutil.copy(javafileSource,javafileTarget)

    #print "copy code to project done!"
# end of copyTextToAndroidmk(context) function
def writeManifest():
    #print "copying Manifest"
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
                <action android:name="android.net.conn.CONNECTIVITY_CHANGE" />
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

    manifest = context["dst_project_path"]+context["dst_project_name"]+"/proj.android/AndroidManifest.xml"
    appKey = context["appkey"]

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
    #print "copy Manifest done"
# end of writeManifest() function
def replacePackageName():
    #print "replacing package name"
    pushserverTarget = context["dst_project_path"]+context["dst_project_name"]+"/proj.android/jni/JPushService.cpp"
    originString = context['dst_package_name']
    nameFunction = originString.replace('.','_');
    path=originString.replace('.','/')
    #nameFunction=re.sub('.','_',originString)
    #namePath=re.sub('.','_',originString)

    fp = file(pushserverTarget)
    content = fp.read();
    content=content.replace('Your Package Name',path)
    content=content.replace('Your_Package_Name',nameFunction)
    content=content.replace('MainActivityName',context["dst_project_name"])
    fp.close()

    fp = file(pushserverTarget,'w')
    fp.write(content)
    fp.close()
    #print "replace package name done"

# end of replacePackageName() function
def fixMainActivity():
    #print "fixing main activity"
    javafileTarget = context["dst_project_path"]+context["dst_project_name"]+"/proj.android/src/"
    path=context['dst_package_name'].replace('.','/')
    javafileTarget=javafileTarget+'/'+path+'/'+context['dst_project_name']+'.java'
    fp=file(javafileTarget)
    lines=[]
    resultExist=0
    i=0
    for line in fp:
        lines.append(line)
        # print "index is %d cotent :%s" %(i,line)
        i+=1
        if line.find("android.content.Context;")>=0:
            resultExist=1
            # print "find target %s" %(line)
    fp.close()
    if resultExist==0:
        i=0
        for line in lines:
            i+=1
            if line.find("import")>=0:
                print("finded import")
                break
        lines.insert(i,"import android.content.Context;\n")
        # print("insert success")
    # print("already include android.content.Context;")
    i=0
    for line in lines:
        i=i+1
        if line.find('{')>=0:
            break;
    lines.insert(i,  "	public static Context STATIC_REF = null;\n")
    lines.insert(i+1,"	public static Context getContext(){\n")
    lines.insert(i+2,"  	return STATIC_REF;\n")
    lines.insert(i+4,"	}\n")
    i=0
    for line in lines:
        i=i+1
        if line.find("super.onCreate")>=0:
            break;
    lines.insert(i, "		STATIC_REF = this.getApplicationContext();\n");
    fp=file(javafileTarget,'w')
    for s in lines:
        fp.write(s)
    fp.close()
    #print "fix main activity done"

# end of fixMainActivity() function
# -------------- main --------------
# dump argvs
# print sys.argv
checkParams(context);
copyToProjcect();
copyTextToAndroidmk();
copyCodeToProject();
writeManifest();
replacePackageName();
fixMainActivity();
print "The JPush SDK has been successfully import in your project,have fun!"

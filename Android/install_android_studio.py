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

def jpushLog(arg):
    print("----- JPush log ----- \n "+arg)
#end of jpushLog function()

def compare_xml(left, right, key_info='.'):
    OK=True  
    main_pid = 10000  
    loop_depth = 0  
    loop_depth += 1  
    # if loop_depth == 1: print  "loop depth==1"
    if left.tag != right.tag:  
        OK = print_diff(main_pid, key_info, 'difftag', left.tag, right.tag)  
        return OK
    if left.text != right.text:  
        OK = print_diff(main_pid, key_info, 'difftext', left.text, right.text)  
        return OK
    leftitems = dict(left.items())
    rightitems = dict(right.items())
    for k,v in leftitems.items():
        if k not in rightitems:
            s = '%s/%s' % (key_info, left.tag)  
            OK=print_diff(main_pid, s, 'lostattr', k, "")
            return OK
        if v!=rightitems[k]:
            s = '%s/%s' % (key_info, left.tag)  
            OK=print_diff(main_pid, s, 'lostattr', k, "")
            return OK
    for k,v in rightitems.items():  
        if k not in leftitems:  
            s = '%s/%s' % (key_info, right.tag)  
            OK=print_diff(main_pid, s, 'extraattr', "", k)
            return OK
        if v!=leftitems[k]:
            s = '%s/%s' % (key_info, right.tag)  
            OK=print_diff(main_pid, s, 'extraattr', "", k)
            return OK
    leftnodes = left.getchildren()  
    rightnodes = right.getchildren()  
    leftlen = len(leftnodes)  
    rightlen = len(rightnodes)  
    if leftlen != rightlen:  
        s = '%s/%s' % (key_info, right.tag)  
        OK=print_diff(main_pid, s, 'difflen', leftlen, rightlen)  
        return OK
    l = leftlen<rightlen and leftlen or rightlen  
    d = {}  
    for i in xrange(l):
        node=leftnodes[i]
        if node.tag not in d:
            d[node.tag] = 1
            tag = node.tag
        else:  
            tag = node.tag + str(d[node.tag])
            d[node.tag] += 1
        s = '%s/%s' % (key_info, tag)  
        OK = compare_xml(leftnodes[i], rightnodes[i], s)
    return OK

def print_diff(main_pid, key_info, msg, base_type, test_type):  
    info = u'[ %-5s ] %s -> %-40s [ %s != %s ]'%(msg.upper(), main_pid, key_info.strip('./'), base_type, test_type)  
    # print info.encode('gbk')
    # print info
    return False  

#end of xml compare function()


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
    context["src_project_path"] = os.getcwd() + "/../Android/"

    #merge path
    context["dst_project_path"]=context["dst_project_path"]+context["dst_project_name"]

# end of checkParams(context) function


def copyToProjcect():
    libSource=context["src_project_path"]+"libs/jpush-sdk-release2.1.5.jar"
    libTarget=context["dst_project_path"]+"/proj.android-studio/app/libs/"
    if not os.path.exists(libTarget):
        os.makedirs(libTarget)
    else:
        for f in glob.glob(libTarget+"/jpush-sdk-release*.jar"):
            os.remove(f)
        for directory in glob.glob(libTarget+"/armeabi/jpush*.*"):
            shutil.rmtree(directory)
    shutil.copy(libSource,libTarget)
    prebuildSource=context["src_project_path"]+"libs/prebuild/"
    prebuildTarget=context["dst_project_path"]+"/proj.android-studio/app/jni/prebuild/"
    if os.path.exists(prebuildTarget):
        shutil.rmtree(prebuildTarget)
    shutil.copytree(prebuildSource,prebuildTarget)

    appLibsTarget=context["dst_project_path"]+"/proj.android-studio/app/libs/"
    copyFiles(prebuildSource,appLibsTarget)

    # soSource=context["src_project_path"]+"libs/prebuild/"
    # v7aSoSource=context["src_project_path"]+"libs/prebuild/armeabi-v7a/libjpush215.so"

    # soTarget=context["dst_project_path"]+"/proj.android-studio/app/libs/armeabi/"
    # v7aTarget=context["dst_project_path"]+"/proj.android-studio/app/libs/armeabi-v7a/"

    # if (os.path.exists(soTarget) == False):
    #     os.makedirs(soTarget)       
    # soTarget+="libjpush215.so"

    # v7aSoTarget=context["dst_project_path"]+"/proj.android/libs/armeabi-v7a"

    # shutil.copyfile(soSource,soTarget)
    # shutil.copytree(v7aSoSource,v7aSoTarget)

# end of copyToProjcect(context) function



def copyTextToAndroidmk():
    mkTargetPath=context["dst_project_path"]+"/proj.android-studio/app/jni/Android.mk"
    fp=file(mkTargetPath)
    lines=[]
    for line in fp: 
        lines.append(line)
    fp.close()
    # delete Android.mk and LOCAL_SHARED_LIBRARIES
    i=0
    for content in lines:
        if content.find("Android.mk") >= 0:
            del lines[i]
            break
        i+=1
    i=0
    for line in lines:
        if line.find("LOCAL_SHARED_LIBRARIES := jpush_so")>=0:
            del lines[i]
            break
        i+=1
    #add new Android.mk file
    i=0
    for content in lines:
        i=i+1
        if content.find("include $(BUILD_SHARED_LIBRARY)") >= 0:
            break
    lines.insert(i, "include $(LOCAL_PATH)/prebuild/Android.mk\n")
    i=i+1
    lines.insert(i, "LOCAL_SHARED_LIBRARIES := jpush_so\n");
    #add jpush service cpp
    isExit=False
    for line in lines:
        if line.find("JPushBridge.cpp")>=0:
            isExit=True
            break
    if isExit==False:
        i=0
        for linesub in lines:
            i=i+1
            if linesub.find("hellocpp/main.cpp")>=0:
                lines.insert(i,"                    JPushBridge.cpp \\\n");
                break
    fp=file(mkTargetPath,'w')
    for s in lines:   
        fp.write(s)
    fp.close()
# end of copyTextToAndroidmk(context) function



def copyCodeToProject():
    hfileSource = context["src_project_path"]+"../JPushBridge.h"
    hfileTarget = context["dst_project_path"]+"/Classes/JPushBridge.h"
    hfileTargetPath = context["dst_project_path"]+"/Classes/"
    if not os.path.exists(hfileTargetPath):
        os.makedirs(hfileTargetPath)
    elif os.path.exists(hfileTarget):
        os.remove(hfileTarget)
    shutil.copy(hfileSource,hfileTarget)

    cppfileSource = context["src_project_path"]+"JPushBridge.cpp"
    cppfileTarget = context["dst_project_path"]+"/proj.android-studio/app/jni/JPushBridge.cpp"
    cppfileTargetPath=context["dst_project_path"]+"/proj.android-studio/app/jni/"
    if not os.path.exists(cppfileTargetPath):
        os.makedirs(cppfileTargetPath)
    elif os.path.exists(cppfileTarget):
        os.remove(cppfileTarget)
    shutil.copy(cppfileSource,cppfileTarget)
    javafileSource = context["src_project_path"]+"JPushCallbackHelper.java"
    javafileTarget = context["dst_project_path"]+"/proj.android-studio/app/src/"
    paths=context["dst_package_name"].split('.');
    for path in paths:
        javafileTarget=javafileTarget+path+"/"
    if os.path.exists(javafileTarget+"JPushCallbackHelper.java"):
        os.remove(javafileTarget+"JPushCallbackHelper.java")
    if not os.path.exists(javafileTarget):
        os.makedirs(javafileTarget)
    shutil.copy(javafileSource,javafileTarget)

# end of copyTextToAndroidmk(context) function


def writeManifest():
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
    <uses-permission android:name="android.permission.WRITE_SETTINGS" />
</manifest>"""

    ET.register_namespace("android","http://schemas.android.com/apk/res/android")

    manifest = context["dst_project_path"]+"/proj.android-studio/app/AndroidManifest.xml"
    appKey = context["appkey"]

    tree_one = ET.parse(manifest)
    first_root = tree_one.getroot()
    package_name = first_root.attrib['package']
        
    data = re.sub('Your_Package_Name',package_name,data,flags=re.I)
    data =re.sub('app_key',appKey,data,flags=re.I)

    tree_two = ET.ElementTree(ET.fromstring(data))
    second_root = tree_two.getroot()

    permission = second_root.findall('permission')
    permissionFirst = first_root.findall("permission")
    addPermission=[]
    for subPermission in permission:
        # print subPermission.tag,subPermission.attrib
        isExist=False
        for subPermissionFirst in permissionFirst:
            if compare_xml(subPermission,subPermissionFirst):
                isExist=True
                break;
        if isExist==False:
            addPermission.append(subPermission)
    first_root.extend(addPermission)

    ele = second_root.find('application')
    app_ele = first_root.find('application')
    addEle=[]
    for subEle in ele:
        # print subEle.tag,subEle.attrib
        isExist=False
        for subApp_ele in app_ele:
            if compare_xml(subEle,subApp_ele):
                isExist=True
                break
        if isExist==False:
            addEle.append(subEle)
    app_ele.extend(addEle)
    #app_ele.extend(ele)

    permission = second_root.findall('uses-permission')
    permissionFirst = first_root.findall("uses-permission")
    addPermission=[]
    i=0
    for subPermission in permission:
        isExist=False
        j=0
        for subPermissionFirst in permissionFirst:
            if compare_xml(subPermission,subPermissionFirst):
                isExist = True
                break
            j+=1
        if isExist==False:
            # print "end a time of compare index :",i,"compare result:",isExist
            addPermission.append(subPermission)
        i+=1
    first_root.extend(addPermission)
    # print "end fix manifest"
#for element in first_root.iter():

    tree = ET.ElementTree(first_root)
    tree.write(manifest,"utf-8",xml_declaration=True)
# end of writeManifest() function

def replacePackageName():
    pushserverTarget = context["dst_project_path"]+"/proj.android-studio/app/jni/JPushBridge.cpp"
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

# end of replacePackageName() function

def fixMainActivity():
    #print "fixing main activity"
    javafileTarget = context["dst_project_path"]+"/proj.android-studio/app/src/"
    for parent,dirnames,filenames in os.walk(javafileTarget):
        for filename in filenames:
            tempDir=os.path.join(parent,filename)
            if tempDir.find("AppActivity.java")>0:
                javafileTarget=tempDir
#    path=context['dst_package_name'].replace('.','/')
#    javafileTarget=javafileTarget+path+'/'+context['dst_project_name']+'.java'
    fp=file(javafileTarget)
    lines=[]
    resultExist=0
    i=0
    for line in fp:
        lines.append(line)
        i+=1
        if line.find("android.content.Context;")>=0:
            resultExist=1
#    fp.close()
    if resultExist==0:
        i=0
        for line in lines:
            i+=1
            if line.find("import")>=0:
                break
        lines.insert(i,"import android.content.Context;\n")

    i=0
    resultExist=0
    for line in fp:
        lines.append(line)
        i+=1
        if line.find("android.os.Bundle;")>=0:
            resultExist=1
    fp.close()
    if resultExist==0:
        i=0
        for line in lines:
            i+=1
            if line.find("import")>=0:
                break
        lines.insert(i,"import android.os.Bundle;\n")

    isAdded=0
    for line in lines:
        if line.find("public static Context STATIC_REF = null")>0:
            isAdded=1;
    if isAdded==0:
        i=0
        for line in lines:
            i=i+1
            if line.find('{')>=0:
                break;
        lines.insert(i,  "      public static Context STATIC_REF = null;\n")
        lines.insert(i+1,"      public static Context getContext(){\n")
        lines.insert(i+2,"      return STATIC_REF;\n")
        lines.insert(i+3,"      }\n\n")
        lines.insert(i+4,"      protected void onCreate(Bundle savedInstanceState) {\n")
        lines.insert(i+5,"      super.onCreate(savedInstanceState);\n")
        lines.insert(i+6,"      STATIC_REF = this.getApplicationContext();\n      }\n")
#        i=0
#        for line in lines:
#            i=i+1
#            if line.find("super.onCreate")>=0:
#                break;
#        lines.insert(i, "       STATIC_REF = this.getApplicationContext();\n");
    fp=file(javafileTarget,'w')
    for s in lines:
        fp.write(s)
    fp.close()

# end of fixMainActivity() function

def fixJPushCallbackHelper():
    javafileTarget = context["dst_project_path"]+"/proj.android-studio/app/src/"
    path=context['dst_package_name'].replace('.','/')
    javafileTarget=javafileTarget+path+'/'+'JPushCallbackHelper.java'
    fp=file(javafileTarget)
    lines=[]
    for line in fp:
        lines.append(line)
    fp.close()
    isExist=0
    for line in lines:
        if line.find("package")>=0:
            isExist=1
            break
    if isExist==0:
        lines.insert(0,"package "+context["dst_package_name"]+";\n")
    fp=file(javafileTarget,"w")
    for line in lines:
        fp.write(line)
    fp.close()
#end of fixJPushCallbackHelper() function


# super copy
def copyFiles(sourceDir, targetDir):
    if sourceDir.find(".svn") > 0:
        return
    for file in os.listdir(sourceDir):
        sourceFile = os.path.join(sourceDir,  file)
        targetFile = os.path.join(targetDir,  file)
        if os.path.isfile(sourceFile):
            if not os.path.exists(targetDir):
                os.makedirs(targetDir)
            if not os.path.exists(targetFile) or(os.path.exists(targetFile) and (os.path.getsize(targetFile) != os.path.getsize(sourceFile))):
                    open(targetFile, "wb").write(open(sourceFile, "rb").read())
        if os.path.isdir(sourceFile):
            First_Directory = False
            copyFiles(sourceFile, targetFile)
# super copy end




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
fixJPushCallbackHelper();
jpushLog("The JPush SDK has been successfully import in your project,have fun!")

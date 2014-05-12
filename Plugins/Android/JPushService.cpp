/*
 * JPushService.cpp
 *
 *  Created on: 2014?4?16?
 *      Author: john
 */

#include "JPushService.h"
#include <jni.h>
#include <platform/android/jni/JniHelper.h>
#include <map>
#include <cocos2d.h>
#include "JPushUtil.h"

using namespace cocos2d;
extern "C"{

#define GET_JSTRING(cstr)   ((cstr) ? t.env->NewStringUTF(cstr) : NULL)

#define SAFE_RELEASE_JOBJ(jobj)   if(jobj) { \
                                    t.env->DeleteLocalRef(jobj); \
                                }
#define SAFE_RELEASE_JCONTEXT(jctx)     if(!s_jpush_context){ \
                                            SAFE_RELEASE_JOBJ(jctx); \
                                }


const char* kJPushClassName = "cn/jpush/android/api/JPushInterface";
const char* kActivityName = "Your Main ActivityName";
const char* kCallbackClassName = "Your Package Name/JPushCallbackHelper";


jobject s_jpush_context = NULL;

jobject getContext(){
    if (s_jpush_context) {
        return s_jpush_context;
    }

    JniMethodInfo t;
    if( JniHelper::getStaticMethodInfo(t
                                       , kActivityName
                                       , "getContext"
                                       , "()Landroid/content/Context;"))
    {
        jobject ret = t.env->CallStaticObjectMethod(t.classID, t.methodID);
        SAFE_RELEASE_JOBJ(t.classID);
        return ret;
    }
    return NULL;
}


void JPushService::init(){
	JniMethodInfo t;
	if ( JniHelper::getStaticMethodInfo(t
									,kJPushClassName
									,"init"
									,"(Landroid/content/Context;)V") ){
		jobject ctx = getContext();
		t.env->CallStaticVoidMethod(t.classID,t.methodID,ctx);
		SAFE_RELEASE_JCONTEXT(ctx);
		SAFE_RELEASE_JOBJ(t.classID);
	}
}

void JPushService::setDebugMode(bool enable){
	JniMethodInfo t;
		if ( JniHelper::getStaticMethodInfo(t
										,kJPushClassName
										,"setDebugMode"
										,"(Z)V") ){
			jboolean isEnable = enable;
			t.env->CallStaticVoidMethod(t.classID,t.methodID,isEnable);
			SAFE_RELEASE_JOBJ(t.classID);
		}
}

void JPushService::stopPush(){
	JniMethodInfo t;
	if(JniHelper::getStaticMethodInfo(t
								,kJPushClassName
								,"stopPush"
								,"(Landroid/content/Context;)V" )){
        jobject ctx = getContext();
        t.env->CallStaticVoidMethod(t.classID,t.methodID,ctx);
        SAFE_RELEASE_JCONTEXT(ctx);
		SAFE_RELEASE_JOBJ(t.classID);
	}
}

void JPushService::resumePush(){
	JniMethodInfo t;
	if( JniHelper::getStaticMethodInfo(t
								,kJPushClassName
								,"resumePush"
								,"(Landroid/content/Context;)V" )){
        jobject ctx = getContext();
        t.env->CallStaticVoidMethod(t.classID,t.methodID,ctx);
        SAFE_RELEASE_JCONTEXT(ctx);
		SAFE_RELEASE_JOBJ(t.classID);
	}
}

bool JPushService::isPushStopped(){
	JniMethodInfo t;
	jboolean isStopped;
	if( JniHelper::getStaticMethodInfo(t
							,kJPushClassName
							,"isPushStopped"
							,"(Landroid/content/Context;)Z" )){
		jobject ctx = getContext();
		isStopped = t.env->CallStaticBooleanMethod(t.classID,t.methodID,ctx);
		SAFE_RELEASE_JCONTEXT(ctx);
		SAFE_RELEASE_JOBJ(t.classID);
	}
	return isStopped;
}

void JPushService::setAliasAndTags(const char *alias, set<string> *tags){
	JniMethodInfo t;
	if ( JniHelper::getStaticMethodInfo(t
									,kJPushClassName
									,"setAliasAndTags"
									,"(Landroid/content/Context;Ljava/lang/String;Ljava/util/HashSet;)V")){

		jstring jalias = GET_JSTRING(alias);
		jobject ctx = getContext();
		jobject jtags = JPushUtil::getJstringSet(tags);
		t.env->CallStaticVoidMethod(t.classID,t.methodID,ctx,jalias,jtags);
		SAFE_RELEASE_JCONTEXT(ctx);
		SAFE_RELEASE_JOBJ(jalias);
		SAFE_RELEASE_JOBJ(jtags);
		SAFE_RELEASE_JOBJ(t.classID);

	}

}

jobject getCallbackHelperObject(){
	JniMethodInfo t;
	jobject callback = NULL;
	if( JniHelper::getMethodInfo(t
								,kCallbackClassName
								,"<init>"
								,"()V")){
		callback = t.env->NewObject(t.classID,t.methodID);
		SAFE_RELEASE_JOBJ(t.classID);
	}
	return callback;
}

void JPushService::setAliasAndTags(void* p_handle,const char *alias, set<string> *tags, APTagAliasCallback callback){
	JniMethodInfo t;
	jobject callbackHelper = getCallbackHelperObject();
	if( JniHelper::getMethodInfo(t
								,kCallbackClassName
								,"setAliasAndTags"
								,"(JLandroid/content/Context;Ljava/lang/String;Ljava/util/Set;J)V" )){

		jstring jalias = GET_JSTRING(alias);
		jobject ctx = getContext();
		jobject jtags = JPushUtil::getJstringSet(tags);
		long func_ptr = (long)callback;
		jlong j_func_ptr = func_ptr;
		long callback_handler = (long)p_handle;
		jlong j_callback_handler = callback_handler;
		t.env->CallObjectMethod(callbackHelper,t.methodID,j_callback_handler,ctx,jalias,jtags,j_func_ptr);
		SAFE_RELEASE_JCONTEXT(ctx);
		SAFE_RELEASE_JOBJ(jalias);
		SAFE_RELEASE_JOBJ(jtags);
		SAFE_RELEASE_JOBJ(callbackHelper);
		SAFE_RELEASE_JOBJ(t.classID);
	}
}

void JPushService::setAlias(void* p_handle,const char *alias, APTagAliasCallback callback){
	JniMethodInfo t;
	jobject callbackHelper = getCallbackHelperObject();
	if( JniHelper::getMethodInfo(t
								,kCallbackClassName
								,"setAlias"
								,"(JLandroid/content/Context;Ljava/lang/String;J)V" )){

			jstring j_alias = GET_JSTRING(alias);
			jobject ctx = getContext();
			long func_ptr = (long)callback;
			jlong j_func_ptr = func_ptr;
			long callback_handler = (long)p_handle;
			jlong j_callback_handler = callback_handler;
			t.env->CallObjectMethod(callbackHelper,t.methodID,j_callback_handler,ctx,j_alias,j_func_ptr);
			SAFE_RELEASE_JCONTEXT(ctx);
			SAFE_RELEASE_JOBJ(j_alias);
			SAFE_RELEASE_JOBJ(callbackHelper);
			SAFE_RELEASE_JOBJ(t.classID);
	}
}

void JPushService::setTags(void* p_handle,set<string> *tags, APTagAliasCallback callback){
	JniMethodInfo t;
	jobject callbackHelper = getCallbackHelperObject();
	if( JniHelper::getMethodInfo(t
									,kCallbackClassName
									,"setTags"
									,"(JLandroid/content/Context;Ljava/util/Set;J)V" )){

			jobject ctx = getContext();
			jobject jtags = JPushUtil::getJstringSet(tags);
			long func_ptr = (long)callback;
			jlong j_func_ptr = func_ptr;
			long callback_handler = (long)p_handle;
			jlong j_callback_handler = callback_handler;
			t.env->CallObjectMethod(callbackHelper,t.methodID,j_callback_handler,ctx,jtags,j_func_ptr);
			SAFE_RELEASE_JCONTEXT(ctx);
			SAFE_RELEASE_JOBJ(jtags);
			SAFE_RELEASE_JOBJ(callbackHelper);
			SAFE_RELEASE_JOBJ(t.classID);
	}
}

set<string> * JPushService::filterValidTags(set<string> *tags){
	JniMethodInfo t;
	set<string> * cpp_filteredTags;
	if ( JniHelper::getStaticMethodInfo(t
								,kJPushClassName
								,"filterValidTags"
								,"(Ljava/util/Set;)Ljava/util/Set;" )){
		jobject jtags = JPushUtil::getJstringSet(tags);
		jobject filteredTags = t.env->CallStaticObjectMethod(t.classID,t.methodID,jtags);
		cpp_filteredTags = JPushUtil::getStdStringSet(filteredTags);
	}
	return cpp_filteredTags;
}


const char * JPushService::registrationID(){
	const char *ret = "";
	JniMethodInfo t;
	if( JniHelper::getStaticMethodInfo(t
								,kJPushClassName
								,"getRegistrationID"
								,"(Landroid/content/Context;)Ljava/lang/String;" ))
	{
		jobject ctx = getContext();
		jstring j_string = (jstring)t.env->CallStaticObjectMethod(t.classID,t.methodID,ctx);
		if(j_string!=NULL){
			string c_string = JniHelper::jstring2string(j_string);
		    ret = c_string.c_str();
		    CCLog("registrationid:%s",ret);
		}
		SAFE_RELEASE_JCONTEXT(ctx);
		SAFE_RELEASE_JOBJ(t.classID);

	}
	return ret;
}

void JPushService::clearAllNotifications(){
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
								,kJPushClassName
								,"clearAllNotifications"
								,"(Landroid/content/Context;)V"))
	{
		jobject ctx = getContext();
		t.env->CallStaticVoidMethod(t.classID,t.methodID,ctx);
		SAFE_RELEASE_JCONTEXT(ctx);
		SAFE_RELEASE_JOBJ(t.classID);
	}
}



void JPushService::clearNotificationById(int notificationId){
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
								,kJPushClassName
								,"clearNotificationById"
								,"(Landroid/content/Context;I)V"))
	{
		jobject ctx = getContext();
		jint notificationId = notificationId;
		t.env->CallStaticVoidMethod(t.classID,t.methodID,ctx,notificationId);
		SAFE_RELEASE_JCONTEXT(ctx);
		SAFE_RELEASE_JOBJ(t.classID);
	}
}

void JPushService::setPushTime(set<int> *weekDays, int startHour, int endHour){
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
								,kJPushClassName
								,"setPushTime"
								,"(Landroid/content/Context;Ljava/util/Set;II)V"))
	{
		jobject ctx = getContext();
		jobject jweekDays = JPushUtil::getJintSet(weekDays);
		jint jstartHour = startHour;
		jint jendHour = endHour;
		t.env->CallStaticVoidMethod(t.classID,t.methodID,ctx,jweekDays,jstartHour,jendHour);
		SAFE_RELEASE_JCONTEXT(ctx);
		SAFE_RELEASE_JOBJ(t.classID);
	}
}

void JPushService::setSilenceTime(int startHour, int startMinute, int endHour, int endMinute){
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
								,kJPushClassName
								,"setSilenceTime"
								,"(Landroid/content/Context;IIII)V"))
	{
		jobject ctx = getContext();
		jint startHour = startHour;
		jint startMinute = startMinute;
		jint endHour = endHour;
		jint endMinute = endMinute;
		t.env->CallStaticVoidMethod(t.classID,t.methodID,ctx,startHour,startMinute,endHour,endMinute);
		SAFE_RELEASE_JCONTEXT(ctx);
		SAFE_RELEASE_JOBJ(t.classID);
	}
} 

void JPushService::setLatestNotifactionNumber(int maxNum){
	JniMethodInfo t;
	if (JniHelper::getStaticMethodInfo(t
								,kJPushClassName
								,"setLatestNotifactionNumber"
								,"(Landroid/content/Context;I)V"))
	{
		jobject ctx = getContext();
		jint j_maxNum = maxNum;
		t.env->CallStaticVoidMethod(t.classID,t.methodID,ctx,j_maxNum);
		SAFE_RELEASE_JCONTEXT(ctx);
		SAFE_RELEASE_JOBJ(t.classID);
	}
}
}

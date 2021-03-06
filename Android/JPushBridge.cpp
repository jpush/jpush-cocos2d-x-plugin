/*
 * JPushBridge.cpp
 *
 *  Created on: 2014?4?16?
 *      Author: john
 */

#include "JPushBridge.h"
#include <jni.h>
#include <platform/android/jni/JniHelper.h>
#include <map>
#include <cocos2d.h>

using namespace cocos2d;
extern "C"{
    
#define GET_JSTRING(cstr)   ((cstr) ? t.env->NewStringUTF(cstr) : NULL)
    
#define SAFE_RELEASE_JOBJ(jobj)   if(jobj) { \
t.env->DeleteLocalRef(jobj); \
}
//#define SAFE_RELEASE_JOBJ(jctx)     if(!s_jpush_context){ \
//SAFE_RELEASE_JOBJ(jctx); \
//}
    
    
const char* kJPushClassName = "cn/jpush/android/api/JPushInterface";
const char* kActivityName = "Your Package Name/MainActivityName";
const char* kCallbackClassName = "Your Package Name/JPushCallbackHelper";


//jobject s_jpush_context = NULL;

jobject getContext(){
//    if (s_jpush_context) {
//        return s_jpush_context;
//    }
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

jobject getJinteger(int value){
    JniMethodInfo t;
    if(JniHelper::getMethodInfo(t
            ,"java/lang/Integer"
            ,"<init>"
            ,"(I)V")){
        jobject ret = t.env -> NewObject(t.classID,t.methodID,value);
        SAFE_RELEASE_JOBJ(t.classID);
        return ret;
    }
    return NULL;
}

jobject getJstringSet(std::set<std::string> *stdSet){
    if (stdSet == NULL) {
        return NULL;
    }
    JniMethodInfo t;
    if(    JniHelper::getMethodInfo(t
            , "java/util/HashSet"
            , "<init>"
            , "(I)V")){
        jobject ret = t.env->NewObject(t.classID, t.methodID, stdSet->size());
        SAFE_RELEASE_JOBJ(t.classID);
        if(JniHelper::getMethodInfo(t
                , "java/util/HashSet"
                , "add"
                , "(Ljava/lang/Object;)Z")){
            for (std::set<std::string>::iterator it = stdSet->begin(); it != stdSet->end(); it++) {
                jstring k = GET_JSTRING(it->c_str());
                t.env->CallObjectMethod(ret, t.methodID, k);
                SAFE_RELEASE_JOBJ(k);
            }
            SAFE_RELEASE_JOBJ(t.classID);
            return ret;
        }
    }
    return NULL;

}

jobject getJintSet(std::set<int> *stdSet){
    if (stdSet == NULL) {
        return NULL;
    }
    JniMethodInfo t;
    if(JniHelper::getMethodInfo(t
            , "java/util/HashSet"
            , "<init>"
            , "(I)V")){
        jobject ret = t.env->NewObject(t.classID, t.methodID,stdSet->size());
        SAFE_RELEASE_JOBJ(t.classID);
        if(JniHelper::getMethodInfo(t
                , "java/util/HashSet"
                , "add"
                , "(Ljava/lang/Object;)Z")){
        	for (std::set<int>::iterator it = stdSet->begin(); it != stdSet->end(); it++) {
        	            jobject k = getJinteger(*it);
        	            t.env->CallObjectMethod(ret, t.methodID, k);
        	            SAFE_RELEASE_JOBJ(k);
        	        }
        	        SAFE_RELEASE_JOBJ(t.classID);
        	        return ret;
        }
    }
    return NULL;
    
}


std::set<std::string> * getStdStringSet(std::set<std::string>*originalSet,jobject object){
    if(object == NULL){
        return NULL;
    }
    
    if(originalSet == NULL){
        originalSet = new std::set<std::string>;
    }else{
        originalSet->empty();
    }
    
    JniMethodInfo t;
    if ( JniHelper::getMethodInfo(t
                                  ,"java/util/HashSet"
                                  ,"iterator"
                                  ,"()Ljava/util/Iterator;")){
        jobject j_iteraror = t.env->CallObjectMethod(object,t.methodID);
        SAFE_RELEASE_JOBJ(t.classID);
        
        if(JniHelper::getMethodInfo(t
                                    ,"java/util/Iterator"
                                    ,"hasNext"
                                    ,"()Z")){
            
            JniMethodInfo ret;
            if( JniHelper::getMethodInfo(ret
                                         ,"java/util/Iterator"
                                         ,"next"
                                         ,"()Ljava/lang/Object;" )){
                while(t.env->CallBooleanMethod(j_iteraror,t.methodID)){
                    jstring string = (jstring)ret.env->CallObjectMethod(j_iteraror,ret.methodID);
                    const char *tag = ret.env->GetStringUTFChars(string, JNI_FALSE);
                    std::string cpp_tag = tag;
                    originalSet->insert(cpp_tag);
                    SAFE_RELEASE_JOBJ(string);
                }
                SAFE_RELEASE_JOBJ(ret.classID);
            }
        }
    }
    return originalSet;
}


void JPushBridge::init(){
    JniMethodInfo t;
    if ( JniHelper::getStaticMethodInfo(t
                                        ,kJPushClassName
                                        ,"init"
                                        ,"(Landroid/content/Context;)V") ){
        jobject ctx = getContext();
        t.env->CallStaticVoidMethod(t.classID,t.methodID,ctx);
        SAFE_RELEASE_JOBJ(ctx);
        SAFE_RELEASE_JOBJ(t.classID);
    }
}

void JPushBridge::setDebugMode(bool enable){
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

void JPushBridge::stopPush(){
    JniMethodInfo t;
    if(JniHelper::getStaticMethodInfo(t
                                      ,kJPushClassName
                                      ,"stopPush"
                                      ,"(Landroid/content/Context;)V" )){
        jobject ctx = getContext();
        t.env->CallStaticVoidMethod(t.classID,t.methodID,ctx);
        SAFE_RELEASE_JOBJ(ctx);
        SAFE_RELEASE_JOBJ(t.classID);
    }
}

void JPushBridge::resumePush(){
    JniMethodInfo t;
    if( JniHelper::getStaticMethodInfo(t
                                       ,kJPushClassName
                                       ,"resumePush"
                                       ,"(Landroid/content/Context;)V" )){
        jobject ctx = getContext();
        t.env->CallStaticVoidMethod(t.classID,t.methodID,ctx);
        SAFE_RELEASE_JOBJ(ctx);
        SAFE_RELEASE_JOBJ(t.classID);
    }
}

bool JPushBridge::isPushStopped(){
    JniMethodInfo t;
    jboolean isStopped;
    if( JniHelper::getStaticMethodInfo(t
                                       ,kJPushClassName
                                       ,"isPushStopped"
                                       ,"(Landroid/content/Context;)Z" )){
        jobject ctx = getContext();
        isStopped = t.env->CallStaticBooleanMethod(t.classID,t.methodID,ctx);
        SAFE_RELEASE_JOBJ(ctx);
        SAFE_RELEASE_JOBJ(t.classID);
    }
    return isStopped;
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

void JPushBridge::setAliasAndTags(void* p_handle,const char *alias, set<string> *tags, APTagAliasCallback callback){
    JniMethodInfo t;
    jobject callbackHelper = getCallbackHelperObject();
    if( JniHelper::getMethodInfo(t
                                 ,kCallbackClassName
                                 ,"setAliasAndTags"
                                 ,"(JLandroid/content/Context;Ljava/lang/String;Ljava/util/Set;J)V" )){
        
        jstring jalias = GET_JSTRING(alias);
        jobject ctx = getContext();
        jobject jtags = getJstringSet(tags);
        long func_ptr = (long)callback;
        jlong j_func_ptr = func_ptr;
        long callback_handler = (long)p_handle;
        jlong j_callback_handler = callback_handler;
        t.env->CallObjectMethod(callbackHelper,t.methodID,j_callback_handler,ctx,jalias,jtags,j_func_ptr);
        SAFE_RELEASE_JOBJ(ctx);
        SAFE_RELEASE_JOBJ(jalias);
        SAFE_RELEASE_JOBJ(jtags);
        SAFE_RELEASE_JOBJ(callbackHelper);
        SAFE_RELEASE_JOBJ(t.classID);
    }
}

void JPushBridge::setAlias(void* p_handle,const char *alias, APTagAliasCallback callback){
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
        SAFE_RELEASE_JOBJ(ctx);
        SAFE_RELEASE_JOBJ(j_alias);
        SAFE_RELEASE_JOBJ(callbackHelper);
        SAFE_RELEASE_JOBJ(t.classID);
    }
}

void JPushBridge::setTags(void* p_handle,set<string> *tags, APTagAliasCallback callback){
    JniMethodInfo t;
    jobject callbackHelper = getCallbackHelperObject();
    if( JniHelper::getMethodInfo(t
                                 ,kCallbackClassName
                                 ,"setTags"
                                 ,"(JLandroid/content/Context;Ljava/util/Set;J)V" )){
        
        jobject ctx = getContext();
        jobject jtags = getJstringSet(tags);
        long func_ptr = (long)callback;
        jlong j_func_ptr = func_ptr;
        long callback_handler = (long)p_handle;
        jlong j_callback_handler = callback_handler;
        t.env->CallObjectMethod(callbackHelper,t.methodID,j_callback_handler,ctx,jtags,j_func_ptr);
        SAFE_RELEASE_JOBJ(ctx);
        SAFE_RELEASE_JOBJ(jtags);
        SAFE_RELEASE_JOBJ(callbackHelper);
        SAFE_RELEASE_JOBJ(t.classID);
    }
}

bool JPushBridge::filterValidTags(set<string> *tags, set<string> *result){
    if(tags == NULL){
        return false;
    }
    if(result->empty()){
        JniMethodInfo t;
        set<string> * cpp_filteredTags;
        if ( JniHelper::getStaticMethodInfo(t
                                            ,kJPushClassName
                                            ,"filterValidTags"
                                            ,"(Ljava/util/Set;)Ljava/util/Set;" )){
            jobject jtags = getJstringSet(tags);
            jobject filteredTags = t.env->CallStaticObjectMethod(t.classID,t.methodID,jtags);
            cpp_filteredTags = getStdStringSet(result,filteredTags);
            SAFE_RELEASE_JOBJ(jtags);
            SAFE_RELEASE_JOBJ(filteredTags)
            return true;
        }
    }
    return false;
}


const char * JPushBridge::registrationID(){
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
        SAFE_RELEASE_JOBJ(ctx);
        SAFE_RELEASE_JOBJ(t.classID);
        
    }
    return ret;
}

void JPushBridge::clearAllNotifications(){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       ,kJPushClassName
                                       ,"clearAllNotifications"
                                       ,"(Landroid/content/Context;)V"))
    {
        jobject ctx = getContext();
        t.env->CallStaticVoidMethod(t.classID,t.methodID,ctx);
        SAFE_RELEASE_JOBJ(ctx);
        SAFE_RELEASE_JOBJ(t.classID);
    }
}



void JPushBridge::clearNotificationById(int notificationId){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       ,kJPushClassName
                                       ,"clearNotificationById"
                                       ,"(Landroid/content/Context;I)V"))
    {
        jobject ctx = getContext();
        jint notificationId = notificationId;
        t.env->CallStaticVoidMethod(t.classID,t.methodID,ctx,notificationId);
        SAFE_RELEASE_JOBJ(ctx);
        SAFE_RELEASE_JOBJ(t.classID);
    }
}

void JPushBridge::setPushTime(set<int> *weekDays, int startHour, int endHour){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       ,kJPushClassName
                                       ,"setPushTime"
                                       ,"(Landroid/content/Context;Ljava/util/Set;II)V"))
    {
        jobject ctx = getContext();
        jobject jweekDays = getJintSet(weekDays);
        jint jstartHour = startHour;
        jint jendHour = endHour;
        t.env->CallStaticVoidMethod(t.classID,t.methodID,ctx,jweekDays,jstartHour,jendHour);
        SAFE_RELEASE_JOBJ(ctx);
        SAFE_RELEASE_JOBJ(t.classID);
    }
}

void JPushBridge::setSilenceTime(int startHour, int startMinute, int endHour, int endMinute){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       ,kJPushClassName
                                       ,"setSilenceTime"
                                       ,"(Landroid/content/Context;IIII)V"))
    {
        jobject ctx = getContext();
        jint j_startHour = startHour;
        jint j_startMinute = startMinute;
        jint j_endHour = endHour;
        jint j_endMinute = endMinute;
        t.env->CallStaticVoidMethod(t.classID,t.methodID,ctx,j_startHour,j_startMinute,j_endHour,j_endMinute);
        SAFE_RELEASE_JOBJ(ctx);
        SAFE_RELEASE_JOBJ(t.classID);
    }
} 

void JPushBridge::setLatestNotifactionNumber(int maxNum){
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t
                                       ,kJPushClassName
                                       ,"setLatestNotifactionNumber"
                                       ,"(Landroid/content/Context;I)V"))
    {
        jobject ctx = getContext();
        jint j_maxNum = maxNum;
        t.env->CallStaticVoidMethod(t.classID,t.methodID,ctx,j_maxNum);
        SAFE_RELEASE_JOBJ(ctx);
        SAFE_RELEASE_JOBJ(t.classID);
    }
}

//将方法中得Your Package Name替换成你自己的包名，如__com_Jpush_Excample__
void Java_Your_Package_Name_JPushCallbackHelper_setAliasAndTagsCallback(JNIEnv *env,jclass,
                                                                                 jlong func_handler,jint resultCode, jstring alias,jobject tags,jlong func_ptr){
    int result = resultCode;
    const char *c_alias = NULL;
    if(alias!=NULL){
        c_alias = env->GetStringUTFChars(alias, JNI_FALSE);
    }
    std::set<std::string> *c_tags = getStdStringSet(NULL,tags);
    long callback_ptr = func_ptr;
    long callback_handler = func_handler;
    APTagAliasCallback callback = (APTagAliasCallback)callback_ptr;
    void * p_handler = (void *)callback_handler;
    if(callback!=NULL){
        callback(p_handler,result,c_alias,c_tags);
    }
    
    if(alias!=NULL){
        env->ReleaseStringUTFChars(alias, c_alias);
    }
    delete(c_tags);
    
}
    
APNetworkDidReceiveMessage_callback notification_callback = NULL;
void * notificationHandler = NULL;
void JPushBridge::registerRemoteNotifcationCallback(void *p_handle, APNetworkDidReceiveMessage_callback message_callback){
    notification_callback = message_callback;
    notificationHandler = p_handle;
}

void Java_Your_Package_Name_JPushReceiver_didReceiveRemoteNotification(JNIEnv *env,jclass,jstring message){
    const char * notification = NULL;
    if(message != NULL){
        notification = env->GetStringUTFChars(message, JNI_FALSE);
    }
    if(notification_callback!=NULL){
        notification_callback(notificationHandler,notification);
    }
    if(message!=NULL){
        env->ReleaseStringUTFChars(message, notification);
    }
}


}

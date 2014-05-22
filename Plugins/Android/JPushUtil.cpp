/*
 * JPushUtil.cpp
 *
 *  Created on: 2014?5?5?
 *      Author: john
 */

#include <JpushUtil.h>
#include <string>

using namespace cocos2d;
extern "C"{

jobject JPushUtil::getJinteger(int value){
	JniMethodInfo t;
	JniHelper::getMethodInfo(t
						,"java/lang/Integer"
						,"<init>"
						,"(I)V");

	jobject ret = t.env -> NewObject(t.classID,t.methodID,value);
	SAFE_RELEASE_JOBJ(t.classID);
	return ret;
}

jobject JPushUtil::getJstringSet(std::set<std::string> *stdSet){
    if (stdSet == NULL) {
        return NULL;
    }
    JniMethodInfo t;
    JniHelper::getMethodInfo(t
                             , "java/util/HashSet"
                             , "<init>"
                             , "(I)V");
    jobject ret = t.env->NewObject(t.classID, t.methodID, stdSet->size());
    SAFE_RELEASE_JOBJ(t.classID);

    JniHelper::getMethodInfo(t
                             , "java/util/HashSet"
                             , "add"
                             , "(Ljava/lang/Object;)Z");

    for (std::set<std::string>::iterator it = stdSet->begin(); it != stdSet->end(); it++) {
        jstring k = GET_JSTRING(it->c_str());
        t.env->CallObjectMethod(ret, t.methodID, k);
        SAFE_RELEASE_JOBJ(k);
    }
    SAFE_RELEASE_JOBJ(t.classID);
    return ret;
}

jobject JPushUtil::getJintSet(std::set<int> *stdSet){
	if (stdSet == NULL) {
	       return NULL;
	}
	JniMethodInfo t;
	JniHelper::getMethodInfo(t
	                      	, "java/util/HashSet"
	                        , "<init>"
	                        , "(I)V");
	jobject ret = t.env->NewObject(t.classID, t.methodID,stdSet->size());
	SAFE_RELEASE_JOBJ(t.classID);

	JniHelper::getMethodInfo(t
	                         , "java/util/HashSet"
	                         , "add"
	                         , "(Ljava/lang/Object;)Z");

	for (std::set<int>::iterator it = stdSet->begin(); it != stdSet->end(); it++) {
		jobject k = getJinteger(*it);
	    t.env->CallObjectMethod(ret, t.methodID, k);
	    SAFE_RELEASE_JOBJ(k);
	}
	SAFE_RELEASE_JOBJ(t.classID);
	return ret;
}


std::set<std::string> * JPushUtil::getStdStringSet(std::set<std::string>*originalSet,jobject object){
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

}


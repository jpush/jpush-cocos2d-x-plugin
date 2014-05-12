/*
 * JPushUtil.h
 *
 *  Created on: 2014?5?5?
 *      Author: john
 */

#ifndef JPUSHUTIL_H_
#define JPUSHUTIL_H_
#include <jni.h>
#include <platform/android/jni/JniHelper.h>
#include <map>
#include <set>
#include <string>
#include <iostream>

#define GET_JSTRING(cstr)   ((cstr) ? t.env->NewStringUTF(cstr) : NULL)

#define SAFE_RELEASE_JOBJ(jobj)   if(jobj) { \
                                    t.env->DeleteLocalRef(jobj); \
                                }
class JPushUtil {
public:
	static jobject getJinteger(int value);
	static jobject getJstringSet(std::set<std::string> *stdSet);
	static jobject getJintSet(std::set<int> *stdSet);
	static std::set<std::string> * getStdStringSet(jobject object);
};

#endif /* JPUSHUTIL_H_ */

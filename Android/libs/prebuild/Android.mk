LOCAL_PATH := $(call my-dir)


include $(CLEAR_VARS)
LOCAL_MODULE := jpush
LOCAL_SRC_FILES := libjpush215.so
include $(PREBUILT_SHARED_LIBRARY)

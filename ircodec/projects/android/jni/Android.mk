LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := libircodec
LOCAL_SRC_FILES := ircodec.c

include $(BUILD_SHARED_LIBRARY)
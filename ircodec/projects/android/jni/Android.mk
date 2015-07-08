LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

IRCODEC_RELATIVE := ./../../..
IRCODEC_SRC :=  $(IRCODEC_RELATIVE)/src
IRCODEC_INC :=  $(IRCODEC_RELATIVE)/include

LOCAL_MODULE    := libircodec

ifneq ("$(GLOBAL_DEFINE)","")
	LOCAL_CFLAGS += -D$(GLOBAL_DEFINE)
endif

LOCAL_C_INCLUDES := $(APP_JNI_PATH)/$(IRCODEC_INC)

LOCAL_SRC_FILES := \
                    $(IRCODEC_SRC)/bitops.c  \
					$(IRCODEC_SRC)/ir_nec.c  \
                    $(IRCODEC_SRC)/ir_unkn0.c  \
                    ircodec.c
					
include $(BUILD_SHARED_LIBRARY)
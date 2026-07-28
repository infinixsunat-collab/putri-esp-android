LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := sock

LOCAL_CPPFLAGS += -pie -fPIE -ffunction-sections -fdata-sections -fvisibility=hidden
LOCAL_LDFLAGS += -pie -fPIE -Wl,--gc-sections
LOCAL_CFLAGS := -Wno-error=format-security -fpermissive
LOCAL_CFLAGS += -ffunction-sections -fdata-sections -fvisibility=hidden
LOCAL_CFLAGS += -fno-rtti -fno-exceptions
LOCAL_CFLAGS += -DNDEBUG
LOCAL_CFLAGS := -O3 -fvectorize -ftree-vectorize -fvisibility=hidden -ffunction-sections -fdata-sections -fvisibility-inlines-hidden -fno-rtti -Wno-error=c++11-narrowing
LOCAL_LDFLAGS := -Wl,-exclude-libs,ALL -Wl,--gc-sections -Wl,--strip-all
LOCAL_CPPFLAGS := -std=c++17 $(LOCAL_CFLAGS)

LOCAL_SRC_FILES := Server/kmods.cpp \

LOCAL_CPP_INCLUDES += $(LOCAL_PATH)
LOCAL_C_INCLUDES += $(LOCAL_PATH)/Server

LOCAL_LDLIBS += -llog

include $(BUILD_EXECUTABLE)

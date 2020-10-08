LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# specify the name of the local module
LOCAL_MODULE    := protected

# specify the src files to include in the native lib
LOCAL_SRC_FILES := $(LOCAL_PATH)/../../$(BUILD_VARIANT)/jni/protected.c

include $(BUILD_SHARED_LIBRARY)
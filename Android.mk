LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := snowcat_static

LOCAL_MODULE_FILENAME := libsnowcat

LOCAL_SRC_FILES := snowcat.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)

################################################################################
define walk
	$(wildcard $(1)) $(foreach e, $(wildcard $(1)/*), $(call walk, $(e)))
endef

# get all src files
ALL_SRC_FILES := $(call walk, $(LOCAL_PATH)/src)

# get all .cpp files
ALL_CPP_FILES := $(filter %.cpp, $(ALL_SRC_FILES))
ALL_SRC_FILES := $(ALL_CPP_FILES:$(LOCAL_PATH)/%=%)

# all include directories
ALL_INC_DIRS := $(shell find $(LOCAL_PATH)/include -type d)
################################################################################

LOCAL_SRC_FILES += $(ALL_SRC_FILES)

LOCAL_C_INCLUDES += $(ALL_INC_DIRS)

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

LOCAL_LDLIBS := -llog

LOCAL_EXPORT_LDLIBS := $(LOCAL_LDLIBS)

LOCAL_WHOLE_STATIC_LIBRARIES := 

include $(BUILD_STATIC_LIBRARY)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := snowcat_static

LOCAL_MODULE_FILENAME := libsnowcat

LOCAL_SRC_FILES := snowcat.cpp \
					\
					src/types/SCObject.cpp \
					src/types/SCArray.cpp \
					src/types/SCDictionary.cpp \
					src/types/SCString.cpp \
					src/types/SCInteger.cpp \
					\
					src/platform/SCFile.cpp \
					src/platform/SCFileManager.cpp \
					src/platform/SCClient.cpp \
					\
					src/data/SCData.cpp \
					src/data/SCDataReader.cpp \
					src/data/SCDataWriter.cpp \
					\
					src/utils/SCStringUtils.cpp \
					src/utils/SCUrlUtils.cpp \
					\
					src/msg/SCNotificationCenter.cpp \
					\
					src/fsm/SCFiniteStateMachine.cpp \
					src/fsm/SCFiniteState.cpp \
					src/fsm/SCStateTransition.cpp \
					src/fsm/SCSequenceTransitions.cpp \
					src/fsm/SCPropertyTransition.cpp \
					\
					src/third_party/MD5.cpp \
					\
					src/SCAutoreleasePool.cpp \
					src/SCLog.cpp 

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
					$(LOCAL_PATH)/include/types \
					$(LOCAL_PATH)/include/utils \
					$(LOCAL_PATH)/include/data \
					$(LOCAL_PATH)/include/http \
					$(LOCAL_PATH)/include/platform \
					$(LOCAL_PATH)/include/zip \
					$(LOCAL_PATH)/include/rtti \
					$(LOCAL_PATH)/include/msg \
					$(LOCAL_PATH)/include/fsm \
					$(LOCAL_PATH)/include/third_party \
					$(LOCAL_PATH)/include 

LOCAL_EXPORT_C_INCLUDES = $(LOCAL_C_INCLUDES)

LOCAL_WHOLE_STATIC_LIBRARIES := 

include $(BUILD_STATIC_LIBRARY)

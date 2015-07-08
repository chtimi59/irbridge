APP_ABI := all

# -------------
# This variable gives the *absolute* path to your application's project root directory.
# This is used to copy/install stripped versions of the generated JNI shared libraries
# to a specific location known by the APK-generating tools.
#
# Fix:
# This flag supposed to be optional but on cygwin it doesn't return an absolute path 
#
# -------------

APP_PROJECT_PATH := $(PWD)
APP_JNI_PATH := $(APP_PROJECT_PATH)/jni/

$(warning "Absolute path used:")
$(warning "     o APP_PROJECT_PATH        : [$(APP_PROJECT_PATH)]")
$(warning "     o APP_JNI_PATH            : [$(APP_JNI_PATH)]")


# Macros to convert cygwin path for Android-NDK (for windows)
# note: needed at least for -L linker option
cygwinpath = $(shell  $(APP_JNI_PATH)/cygwinpath.sh $1 -fake)
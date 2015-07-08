#!/bin/bash

#
# Replace a cygwin path by something understandable for Androind NDK for windows
# 
# Example
# ./cygwinpath.sh .       returns 'c:\dev\irbridge\ircodec\projects\android\jni'
# ./cygwinpath.sh . -fake returns 'c:/dev/irbridge/ircodec/projects/android/jni'
#

LINUX_PATH="$(readlink -f "$1")"

if [ "$(echo "$LINUX_PATH" | grep "/cygdrive")" ]; then
  #replace '/cygdrive/c' by 'c:'
  WINDOWS_PATH="$(echo "$LINUX_PATH" | sed 's/^\/cygdrive\/\(.\)/\1:/g')"
else
  #replace root '/' by 'c:/cygwin'
  WINDOWS_PATH="c:/cygwin$LINUX_PATH"
fi

if [[ $2 =~ "-fake" ]]; then
	WINDOWS_PATH=$WINDOWS_PATH
else
	#replace '/' by '\'
	WINDOWS_PATH="$(echo "$WINDOWS_PATH" | sed 's/\//\\/g')"
fi


echo "$WINDOWS_PATH"
#!/bin/bash
EXPORT_LIBPATH=../../lib
PRE_BUILD_FLAG=

[[ $@ =~ "-V" ]] && PRE_BUILD_FLAG=$PRE_BUILD_FLAG" V=1 "
[[ $@ =~ "-B" ]] && PRE_BUILD_FLAG=$PRE_BUILD_FLAG" -B "

#multiprocessor ?
#BUILD_FLAG="$PRE_BUILD_FLAG -j8"
BUILD_FLAG="$PRE_BUILD_FLAG"

if "$NDK_ROOT"/ndk-build $BUILD_FLAG; then
	mkdir -p $EXPORT_LIBPATH
	cp -rf libs/* $EXPORT_LIBPATH
		echo "******* succeed ********"
	else
		echo "******* failed ********"
		exit 1;
fi


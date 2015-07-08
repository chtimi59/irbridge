#!/bin/bash
ROOTPATH=../..
SRCPATH=$ROOTPATH/src
LIBPATH=$ROOTPATH/lib
INCPATH=$ROOTPATH/include


if "$NDK_ROOT"/ndk-build $BUILD_FLAG; then
	mkdir -p $LIBPATH
	cp -rf libs/* $LIBPATH
		echo "******* succeed ********"
	else
		echo "******* failed ********"
		exit 1;
fi


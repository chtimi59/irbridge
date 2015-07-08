#!/bin/bash
PRE_BUILD_FLAG=
[[ $@ =~ "-V" ]] && PRE_BUILD_FLAG=$PRE_BUILD_FLAG" V=1 "
[[ $@ =~ "-B" ]] && PRE_BUILD_FLAG=$PRE_BUILD_FLAG" -B "

BUILD_FLAG="$PRE_BUILD_FLAG"

export GLOBAL_DEFINE="UNITARY_TEST"

"$NDK_ROOT"/ndk-build $BUILD_FLAG

mkdir -p res
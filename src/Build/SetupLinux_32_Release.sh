#!/bin/bash

workdir=$(cd $(dirname $0); pwd)

buildDir=$workdir/../build_32_Release

if [ ! -d $buildDir ];then
	mkdir -p $buildDir
fi

cd $buildDir

echo Setup Linux 32 bit Release Solution in $(pwd)

cmake -DUSE_32BITS=ON -DCMAKE_BUILD_TYPE=Release ../Plugin

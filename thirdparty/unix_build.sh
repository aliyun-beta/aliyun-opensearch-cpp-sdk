#!/usr/bin/env bash

if [ "$1" == "curl" ]; then
    echo build CURL
    cd ./curl-7.45.0/
    ./configure
    make
    cd ..
elif [ "$1" == "apr" ]; then
    echo build APR
    cd ./apr-1.5.2/
    ./configure
    make
    cd ..
elif [ "$1" == "apu" ]; then
    echo build APR-UTIL
    cd ./apr-util-1.5.4/
    ./configure --with-apr=../apr-1.5.2/
    make
    cd ..
fi

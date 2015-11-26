#!/bin/sh

wget https://github.com/google/googletest/archive/release-1.6.0.tar.gz
tar xzvf release-1.6.0.tar.gz
mv googletest-release-1.6.0 googletest

wget https://github.com/bagder/curl/releases/download/curl-7_45_0/curl-7.45.0.tar.gz
wget http://mirrors.hust.edu.cn/apache/apr/apr-1.5.2.tar.gz
wget http://mirrors.hust.edu.cn/apache/apr/apr-util-1.5.4.tar.gz

tar xzvf curl-7.45.0.tar.gz
tar xzvf apr-1.5.2.tar.gz
tar xzvf apr-util-1.5.4.tar.gz

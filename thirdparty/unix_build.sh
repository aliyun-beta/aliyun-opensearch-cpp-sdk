#!/bin/sh

cd ./curl-7.45.0/
./configure
make
#sudo make install
cd ..

cd ./apr-1.5.2/
./configure
make
#sudo make install
cd ..

cd ./apr-util-1.5.4/
./configure --with-apr=../apr-1.5.2/
make
#sudo make install
cd ..

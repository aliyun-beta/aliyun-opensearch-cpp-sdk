#!/bin/bash
rm -rf build

# configure
cmake -H. -Bbuild -DCMAKE_CXX_FLAGS=--coverage

cd build

# building
make

# run test
./tests/unittests

# collect raw coverage data
lcov -c -d . -o test.info

# filter coverage data of system library, thirdparty, tests
lcov -r test.info '/usr/include/*' 'thirdparty/*' 'tests/*' -o filtered.info

# generate coverage report
genhtml -o html filtered.info

# perform it !
firefox html/index.html &


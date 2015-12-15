#!/bin/sh

#filter some rule we needn't
HEADER_FILTER='--filter=-runtime/int,-runtime/explicit'
SOURCE_FILTER='--filter=-runtime/int,-runtime/string,-build/c++11'
TESTES_FILTER='--filter=-whitespace/line_length'

#use default rule set:
#  ./lint.sh verbose
if [ "$1" = "verbose" ]; then
    HEADER_FILTER=''
    SOURCE_FILTER=''
    TESTES_FILTER=''
fi

# for headers
echo '#######################################################'
find include -name *.h | xargs ./cpplint.py --root=include $HEADER_FILTER

# for sources
echo '#######################################################'
find src -name *.cc | xargs ./cpplint.py $SOURCE_FILTER

# for tests
echo '#######################################################'
find tests -name *.cc | xargs ./cpplint.py $TESTES_FILTER


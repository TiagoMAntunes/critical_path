#!/bin/bash
### use: ./all.sh <compiled program> <sequence of tests>

tests=*.in
prog=$1
if [ $# -gt 1 ]; then
	shift
	tests=$*
fi

for arg in $tests
do
	echo ""
  	echo "Running $arg"
  	time $prog < "${arg%.*}".in > "${arg%.*}".mine.out
  	diff -q "${arg%.*}".out "${arg%.*}".mine.out
done

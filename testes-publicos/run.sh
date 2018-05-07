#!/bin/bash
### use: ./run.sh <program.c> <sequence of tests>

gcc -o bla $1 -Wall -ansi -pedantic
shift
for arg in "$@"
do
	echo ""
  	echo "Running $arg"
  	time ./bla < "${arg%.*}".in > "${arg%.*}".mine.out
  	diff -q "${arg%.*}".out "${arg%.*}".mine.out
done

#rm *.mine.out
rm bla

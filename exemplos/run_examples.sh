#!/bin/bash
### use: ./run_examples <program.c>

gcc -Wall -ansi -pedantic $1 -o proj2
./proj2 < ex0.in > ex0.mine.out; diff -q ex0.out ex0.mine.out
./proj2 < ex1.in > ex1.mine.out; diff -q ex1.out ex1.mine.out
./proj2 < ex2.in > ex2.mine.out; diff -q ex2.out ex2.mine.out
./proj2 < ex3.in > ex3.mine.out; diff -q ex3.out ex3.mine.out
./proj2 < ex4.in > ex4.mine.out; diff -q ex4.out ex4.mine.out

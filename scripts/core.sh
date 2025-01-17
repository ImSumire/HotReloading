#!/bin/sh

g++ -Og -g -o dist/core src/core.cpp dist/libraylib.so.??? -lltdl -ldl -lm

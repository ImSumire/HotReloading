#!/bin/sh

g++ -Og -g -fPIC -shared -o dist/libhot.so src/lib.cpp dist/libraylib.so.??? -lm

#!/bin/bash
g++ -Wall -O2 -g -Wpointer-arith -Wno-unused-variable -Wno-unused-function main_cache.cpp cache.cpp wrapper.cpp exc.cpp cache_test.cpp -o cache
./cache

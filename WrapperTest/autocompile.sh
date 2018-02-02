#!/bin/bash

g++ -fPIC -c WrapperTest.cpp
g++ -fPIC -c WrapperTest_wrap.cxx $(python-config --cflags)
g++ -shared WrapperTest.o WrapperTest_wrap.o $(python-config --ldflags) -o _WrapperTest.so

#!/bin/bash

#Use SWIG to generate wrapper code
swig -c++ -python DriveTrain.i

#Compile original file and wrapper code
g++ -fpic -c EposDriveTrain.cpp -I /usr/include -L/usr/lib -lEposCmd
g++ -fpic -c DriveTrain_wrap.cxx $(python-config --cflags) -L/usr/lib -lEposCmd

#Link objects into shared object to be used as a Python module
g++ -shared EposDriveTrain.o DriveTrain_wrap.o $(python-config --ldflags) -L/usr/lib -lEposCmd -o _DriveTrain.so

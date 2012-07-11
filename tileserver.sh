#! /bin/sh

LD_LIBRARY_PATH=/home/klee/oblong/g-speak-64-2/lib; export LD_LIBRARY_PATH

killall -q -u klee -9 python

cd ~/project/nga/python-plasma
~/install/python-plasma/bin/python receive.py

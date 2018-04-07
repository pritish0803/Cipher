#! /bin/csh

ns wrls-aodv.tcl
exec awk -f getRatio.awk tracef.tr >sam.tr &

g++ test.cpp
./a.out

cat sam.tr &


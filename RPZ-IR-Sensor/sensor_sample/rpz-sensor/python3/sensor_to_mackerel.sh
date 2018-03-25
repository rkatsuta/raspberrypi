#!/bin/sh
home="/home/rkatsuta/sensor"
monitor_time=`date +%s`
temp=`python3 $home/rpz_sensor.py | grep Temp | awk '{print $3}' | perl -pe 's/C//'`
lux=`python3 $home/rpz_sensor.py | grep Lux | perl -pe 's/^.+: (.+)lux/$1/'`
echo "lux\t$lux\t$monitor_time"
echo "temp\t$temp\t$monitor_time"

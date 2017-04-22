#!/bin/bash

if [ "x$1" == "x" ]
then
	echo "Usage: sudo ./script freq"
	echo "Available frequencies are"
	cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies
	exit -1
fi

for cpu in /sys/devices/system/cpu/cpu*/cpufreq/scaling_governor
do 
	echo "userspace" > $cpu 
	echo $cpu
done

for cpu in /sys/devices/system/cpu/cpu*/cpufreq/scaling_min_freq
do 
	echo $1 > $cpu 
	echo $cpu
done

for cpu in /sys/devices/system/cpu/cpu*/cpufreq/scaling_max_freq
do 
	echo $1 > $cpu 
	echo $cpu
done

#!/bin/bash
hciconfig hci0 noscan
echo 14 > /sys/class/gpio/export
echo out > /sys/class/gpio/gpio14/direction
echo 1 > /sys/class/gpio/gpio14/value

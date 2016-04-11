#!/bin/bash
hciconfig hci0 noscan
echo 1 > /sys/class/gpio/gpio14/value

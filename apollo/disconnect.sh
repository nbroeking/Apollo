#!/bin/bash
hciconfig hci0 piscan

echo 0 > /sys/class/gpio/gpio14/value

#!/bin/sh

set -e

PATH=/sbin:/bin:/usr/sbin:/usr/bin

# $METHOD = none, dhcp, static or loopback
# $IFACE = wlan0, --all
# $MODE = start

if [ "$IFACE" = --all ] || [ "$IFACE" = wlan0 ]; then
   sudo /usr/sbin/irbrgd start
fi



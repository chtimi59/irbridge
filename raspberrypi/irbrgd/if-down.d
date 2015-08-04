#!/bin/sh

set -e

PATH=/sbin:/bin:/usr/sbin:/usr/bin

# $METHOD = none, dhcp, static or loopback
# $IFACE = wlan0, --all
# $MODE = start

if [ "$IFACE" = wlan0 ]; then
   /usr/sbin/irbrgd stop
fi



#!/bin/sh

set -e

PATH=/sbin:/bin:/usr/sbin:/usr/bin

# $METHOD = none, dhcp, static or loopback
# $IFACE = wlan0, --all
# $MODE = start

#if [ "$IFACE" = --all ]; then
if [ "$IFACE" = wlan0 ]; then
   /usr/sbin/irbrgd start
fi



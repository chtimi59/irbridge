#!/bin/sh

set -e

PATH=/sbin:/bin:/usr/sbin:/usr/bin

# $METHOD = dhcp, static or loopback

if [ "$METHOD" = static ]; then
	exit 0
fi

if [ "$METHOD" = loopback ]; then
	exit 0
fi

/usr/sbin/irbrgd start

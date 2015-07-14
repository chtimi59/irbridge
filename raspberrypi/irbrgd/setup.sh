#!/bin/sh

mkdir -p /etc/irbrgd

cp irbrgd.conf /etc/irbrgd
chmod a-x /etc/irbrgd/irbrgd.conf

cp if-up.d /etc/network/if-up.d/irbrgd
chmod a+x /etc/network/if-up.d/irbrgd

cp irbrgd /usr/sbin/
chmod a+x /usr/sbin/irbrgd



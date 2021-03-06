#!/bin/sh

mkdir -p /etc/irbrgd

cp irbrgd.conf /etc/irbrgd
chmod a-x /etc/irbrgd/irbrgd.conf

cp if-up.d /etc/network/if-up.d/irbrgd
chmod a+x /etc/network/if-up.d/irbrgd

cp if-down.d /etc/network/if-down.d/irbrgd
chmod a+x /etc/network/if-down.d/irbrgd

cp irbrgd /usr/sbin/
chmod a+x /usr/sbin/irbrgd

#Service to reconnect wlan0 if unassociated
#cp wlan_reconnect.sh /etc/init.d/. 
#chmod a+x /etc/init.d/wlan_reconnect.sh
#update-rc.d wlan_reconnect.sh defaults
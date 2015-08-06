#!/bin/sh -e
### BEGIN INIT INFO
# Provides:          wlan_reconnect.sh
# Required-Start:    
# Required-Stop:
# Default-Start:     S
# Default-Stop:
# Short-Description: Reconnect wifi if unassociated
# Description:       Reconnect wifi if unassociated
### END INIT INFO

daemon_NAME="wlan_reconnect.sh"
export IFACE=wlan0
export PIDFILE=/tmp/wlan_reconnect
PATH="/sbin:/bin:/usr/sbin:/usr/bin"

#install
#---------
#sudo cp wlan_reconnect.sh /etc/init.d/. && sudo chmod a+x /etc/init.d/wlan_reconnect.sh
#sudo update-rc.d wlan_reconnect.sh defaults

#to uninstall
#---------
#sudo update-rc.d -f sabnzbd remove

. /lib/lsb/init-functions

d_start () {
	while [ true ]
	do
		if [ ! -z "`iwconfig $IFACE | grep unassociated`" ]; then
		   sudo ifup --force $IFACE
		fi	
		sleep 60
	done
}

d_stop () {
	if [ -e $PIDFILE ]; then
		kill `cat $PIDFILE`
		rm -f $PIDFILE
	fi
}

case "$1" in

        start)
			if [ -e $PIDFILE ]; then
				log_daemon_msg "Daemon $daemon_NAME already running"
				log_end_msg $?
			else
				log_daemon_msg "Starting system $daemon_NAME Daemon"
				log_end_msg $?				
				d_start &
				echo $! > $PIDFILE
			fi
			;;
			
        stop)
			if [ -e $PIDFILE ]; then
				log_daemon_msg "Stopping system $daemon_NAME Daemon"
				log_end_msg $?
                d_stop
			fi
			;;

        restart|reload|force-reload)
				d_stop
				d_start
                ;;

        force-stop)
               d_stop
                killall -q $daemon_NAME || true
                sleep 2
                killall -q -9 $daemon_NAME || true
                ;;

        status)
                status_of_proc "$daemon_NAME" && exit 0 || exit $?
                ;;
        *)
                echo "Usage: /etc/init.d/$daemon_NAME {start|stop|force-stop|restart|reload|force-reload|status}"
                exit 1
                ;;
esac
exit 0

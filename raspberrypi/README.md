IR-Bridge Project / RaspberryPi
================================

A daemon TCP server to offer a raw Infrared to TCP bridge

Tested on Raspberry PI modele B Rev2

Distrib: raspdebian 3.12.35
https://www.raspberrypi.org/downloads/


Abstract
========

This daemon forward lirc waveform to all TCP client, and send to lirc any data received.
The idea here is to keep a raw waveform in both way, to let client implement their own ir protocol
A lib with some common protocol (NEC, SIRC) are also provided in parent folder (ircodec)


History
=======

2015/07 : Jan dOrgeville - first issue

Prequists: lirc
================
```
sudo apt-get install lirc
```
and then add the following line in /etc/modules (depending on your GPIO setup)
```
lirc_dev
lirc_rpi gpio_in_pin=23 gpio_out_pin=22
```

more info here: http://alexba.in/blog/2013/01/06/setting-up-lirc-on-the-raspberrypi/  
lirc sources before raspberry patches: http://lirc.sourceforge.net/ 

Once your lirc is fully functionnal, remove the default lirc start script
sudo mv /etc/init.d/lirc ~ 

	
folders :
=========
* ir-test0 : really first test with lirc driver, it shows pulse/space waveform when an infrared activity is detected
* ir-test1 : show NEC code when a NEC-based remote is used
* ir-test2 : show/send NEC code when user press a key (see main.c)
* ir-test3 : same as ir-test2 but with a starfrit Scale 39bits propriatery IR protocol
* server   : a first version of a TCP/IR bridge
* irbrgd   : the final project which is basically the daemon version of the previous project

irbrgd :
=========

```
cd irbrgd
make && sudo ./setup.sh
```

see self explained configuration file: /etc/irbrgd/irbrgd.conf

To manually stop the daemon:

```
irbrgd stop
```



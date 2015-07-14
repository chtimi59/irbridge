IR-Bridge Project
================================

A raw Infrared to TCP bridge

Tested on Raspberry PI modele B Rev2

Distrib: raspdebian 3.12.35
https://www.raspberrypi.org/downloads/

based on lirc for raspberry
http://alexba.in/blog/2013/01/06/setting-up-lirc-on-the-raspberrypi/  


Abstract
========

The project consist mainly in 3 parts
* 'irbrgd' server for Raspberry which provides a raw infrared bridge
* 'ircodec' cross-platform library to encode/decode raw stream
* Some clients samples (Android, linux, ...)

History
=======

2015/07 : Jan dOrgeville - first issue

	
folders :
=========
* raspberrypi : irbrgd server project
* ircodec     : cross-platform library 
* clients     : irbrgd client sample

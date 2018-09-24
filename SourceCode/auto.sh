#!/bin/bash
clear & clear
sudo insmod ./cryptomodule.ko key="123456789012345"
sudo rmmod cryptomodule

sudo insmod ./cryptomodule.ko key="1234567890"
sudo rmmod cryptomodule

sudo insmod ./cryptomodule.ko key="1234567890123456789"
sudo rmmod cryptomodule

journalctl --since="10 seconds ago"| grep kernel

#!/bin/bash
clear & clear

# Key < 16
sudo insmod ./cryptomodule.ko key="1234567890"
sudo rmmod cryptomodule
# Key > 16
sudo insmod ./cryptomodule.ko key="1234567890123456789"
sudo rmmod cryptomodule
# Key == 16
sudo insmod ./cryptomodule.ko key="1234567890123456"
sudo ./test_cryptomodule
sudo rmmod cryptomodule
journalctl --since="10 seconds ago"| grep kernel

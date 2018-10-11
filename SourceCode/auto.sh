#!/bin/bash

make
printf "\n============================================================\n"

# # Key < 32
# sudo insmod ./cryptomodule.ko key="00010203040506070809"
# sudo rmmod cryptomodule
#
# # # Key > 32
# sudo insmod ./cryptomodule.ko key="000102030405060708090A0B0C0D0E0F10111213"
# sudo rmmod cryptomodule

# Key == 32
sudo insmod ./cryptomodule.ko key="000102030405060708090A0B0C0D0E0F"

# sudo ./test_cryptomodule "h Felipe Ferreira"
# sudo ./test_cryptomodule "h Felipe Moreira Ferreira"
# sudo ./test_cryptomodule "h 012345678901234567890123456789"
# sudo ./test_cryptomodule "h Felipe Moreira Ferreira"
sudo ./test_cryptomodule "c Felipe Ferreira"
sudo ./test_cryptomodule "c Felipe Moreira Ferreira0"

sudo rmmod cryptomodule

printf "\n============================================================\n"

journalctl --since="10 seconds ago"| grep kernel
# dmesg | grep cryptomodule | tail

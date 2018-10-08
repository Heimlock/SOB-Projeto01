#!/bin/bash

make
printf "\n============================================================\n"

# # Key < 16
# sudo insmod ./cryptomodule.ko key="1234567890"
# sudo rmmod cryptomodule
#
# # Key > 16
# sudo insmod ./cryptomodule.ko key="1234567890123456789"
# sudo rmmod cryptomodule

# Key == 16
sudo insmod ./cryptomodule.ko key="1234567890123456"

# sudo ./test_cryptomodule "h Felipe Ferreira"
# sudo ./test_cryptomodule "h Felipe Moreira Ferreira"
# sudo ./test_cryptomodule "h 012345678901234567890123456789"
# sudo ./test_cryptomodule "h Felipe Moreira Ferreira"
sudo ./test_cryptomodule "c Felipe Ferreira"
# sudo ./test_cryptomodule "c Felipe Moreira Ferreira0"

sudo rmmod cryptomodule

printf "\n============================================================\n"

journalctl --since="10 seconds ago"| grep kernel
# dmesg | grep cryptomodule | tail
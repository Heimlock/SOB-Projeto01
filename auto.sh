#!/bin/bash

make
printf "\n============================================================\n"

sudo insmod ./cryptomodule.ko key="0123456789ABCDEF"

sudo ./test_cryptomodule "c Pelo amor de Deus funciona"


sudo rmmod cryptomodule

printf "\n============================================================\n"

#dmesg
#journalctl --since="10 seconds ago"| grep kernel
# dmesg | grep cryptomodule | tail
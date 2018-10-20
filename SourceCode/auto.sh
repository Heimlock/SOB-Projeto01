#!/bin/bash

#
#		Sistemas Operacionais B
#		Projeto 01 - Módulo Criptográfico
#
#	Integrantes:
#		Bruno Pereira Bannwart 				RA: 15171572
#		Felipe Moreira Ferreira 		 	RA: 16116469
#		Luiz Felipe Zerbetto Masson 	RA: 15166804
#		Matheus Manganeli de Macedo 	RA: 16250276
#		Rodrigo da Silva Cardoso 			RA: 16430126
#
#	  Script que Automatiza os Testes do Módulo
#

make
printf "\n============================================================\n"

# Not a Valid Key
sudo insmod ./cryptomodule.ko key="Teste123"
sudo rmmod cryptomodule

# Key < 32
sudo insmod ./cryptomodule.ko key="00010203040506070809"
sudo rmmod cryptomodule

# Key > 32
sudo insmod ./cryptomodule.ko key="000102030405060708090A0B0C0D0E0FEEEEEEEEEE"
sudo rmmod cryptomodule

# Key == 32
sudo insmod ./cryptomodule.ko key="000102030405060708090A0B0C0D0E0F"

# Not a Valid Operation
sudo ./test_cryptomodule "x 00000000000000000000000000000000"

# Not a Valid Input
sudo ./test_cryptomodule "h Teste123"

# Input Hasn't Even Lenght
sudo ./test_cryptomodule "h 546573746531323"

# # The quick brown fox jumps over the lazy dog -- 43 Letters
# # 54686520717569636b2062726f776e20666f78206a756d7073206f76657220746865206c617a7920646f67
# # Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aliquam hendrerit lorem augue. In sollicitudin libero convallis leo accumsan tempus. Vestibulum posuere nisi scelerisque eleifend lobortis. Mauris felis lectus, dapibus ac.   --  229 Letters
# # 4c6f72656d20697073756d20646f6c6f722073697420616d65742c20636f6e73656374657475722061646970697363696e6720656c69742e20416c697175616d2068656e647265726974206c6f72656d2061756775652e20496e20736f6c6c696369747564696e206c696265726f20636f6e76616c6c6973206c656f20616363756d73616e2074656d7075732e20566573746962756c756d20706f7375657265206e697369207363656c6572697371756520656c656966656e64206c6f626f727469732e204d61757269732066656c6973206c65637475732c20646170696275732061632e

# Hash Operation -- Input Lenght is Bigger than 32Bytes
# sudo ./test_cryptomodule "h 54686520717569636b2062726f776e20666f78206a756d7073206f76657220746865206c617a7920646f67"
## Result:  5ebb50b61fb70c1d94d6e5d1dffcda0aa47f16dd5ebf0ac1be9edc7d639a3abb

sudo ./test_cryptomodule "h 5465737465313233"   ##  Teste123
sudo ./test_cryptomodule "c 5465737465313233"   ##  Teste123
sudo ./test_cryptomodule "d 4B4B909AE6FA1FC50809BC8260430757"

sudo rmmod cryptomodule
printf "\n============================================================\n"

# journalctl --since="10 seconds ago"| grep kernel
# dmesg | grep cryptomodule | tail

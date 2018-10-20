
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
#	  Makefile Responsável pela Compilação Módulo e do Programa Teste
#

# cryptomodule FileOperations main Commonlib sumHash Cipher AuxiliaryFunctions

obj-m 								:=	cryptomodule.o
CFLAGS_cryptomodule.o :=	-DDEBUG
cryptomodule-objs 		:=	main.o FileOperations.o sumHash.o Cipher.o AuxiliaryFunctions.o

all:
	clear
	clear
	@rm -f cryptomodule.ko test_cryptomodule
	gcc test_cryptomodule.c -o test_cryptomodule
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	@rm -f *.mod.c *.mod.o *.o

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f other/ioctl other/cat_noblock *.plist test_cryptomodule

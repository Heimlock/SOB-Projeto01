# cryptomodule FileOperations main Commonlib

obj-m 					:=	cryptomodule.o
CFLAGS_cryptomodule.o 	:=	-DDEBUG
cryptomodule-objs 		:=	main.o

all:
	clear
	clear
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	@rm -f *.mod.c *.mod.o *.o 

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f other/ioctl other/cat_noblock *.plist test_cryptomodule
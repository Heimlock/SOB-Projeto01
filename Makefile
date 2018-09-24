obj-m += cryptomodule.o
cryptomodule-objs := init_crypto.o cleanup_crypto.o device_open.o device_release.o 

all:
	clear
	clear
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	@rm -f *.mod.c *.mod.o *.o

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f other/ioctl other/cat_noblock *.plist
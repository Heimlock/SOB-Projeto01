#
#		Sistemas Operacionais B
#		Atividade 02 - Implementação, Compilação e Teste de Módulos no Kernel Linux
#
#	Integrantes:
#		Bruno Pereira Bannwart 				RA: 15171572
#		Felipe Moreira Ferreira 		 	RA: 16116469
#		Luiz Felipe Zerbetto Masson 		RA: 15166804
#		Matheus Manganeli de Macedo 		RA: 16250276
#		Rodrigo da Silva Cardoso 			RA: 16430126
#
#	Makefile
#
#obj-m += hello-1.o
#obj-m += hello-2.o
#obj-m += hello-3.o
#obj-m += hello-4.o
obj-m += validet_Function.o

#obj-m += chardev.o

#obj-m += startstop.o
#startstop-objs := start.o stop.o

#obj-m += procfs1.o
#CFLAGS_procfs1.o := -DDEBUG
#obj-m += procfs2.o
#CFLAGS_procfs2.o := -DDEBUG
#obj-m += procfs3.o
#CFLAGS_procfs3.o := -DDEBUG
#obj-m += procfs4.o
#CFLAGS_procfs4.o := -DDEBUG

all:
	clear
	clear
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	@rm -f *.mod.c *.mod.o *.o

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f other/ioctl other/cat_noblock *.plist

obj-m+=ledstrip.o
ccflags-y := -std=gnu99
 
all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
load:
	sudo insmod ledstrip.ko
unload:
	sudo rmmod ledstrip.ko

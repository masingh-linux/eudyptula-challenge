obj-m += misc_driver_1.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

remove:
	sudo rmmod misc_driver_1

insert: all
	sudo insmod misc_driver_1.ko

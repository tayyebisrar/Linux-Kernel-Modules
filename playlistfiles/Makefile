obj-m += hello.o
obj-m += hello1.o
obj-m += cdevagain.o
obj-m += chardevcomp.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

KERNEL_TOP ?= /lib/modules/$(shell uname -r)/build

obj-m += dependent_mod.o

all: modules

modules: 
	make -C $(KERNEL_TOP) M=$(PWD) modules

test: 
	make -C $(PWD)/testing M=$(PWD)/testing test

clean:
	make -C $(KERNEL_TOP) M=$(PWD) clean

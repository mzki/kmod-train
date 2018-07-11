KERNEL_TOP ?= /lib/modules/$(shell uname -r)/build

OBJ_NAME=dep_timer

obj-m += $(OBJ_NAME).o

PWD = $(shell pwd)

.PHONY: all modules install uninstall test clean

all: modules

modules: 
	make -C $(KERNEL_TOP) M=$(PWD) modules

install: modules
	sudo insmod $(OBJ_NAME).ko

uninstall: 
	sudo rmmod $(OBJ_NAME)

test: 
	make -C $(PWD)/testing M=$(PWD)/testing test

clean:
	make -C $(KERNEL_TOP) M=$(PWD) clean


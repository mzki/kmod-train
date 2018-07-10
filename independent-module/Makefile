KERNEL_TOP ?= /lib/modules/$(shell uname -r)/build

obj-m += independent_mod.o

all: modules

modules: 
	make -C $(KERNEL_TOP) M=$(CURDIR) modules

test: 
	make -C $(CURDIR)/testing M=$(CURDIR)/testing test

clean:
	make -C $(KERNEL_TOP) M=$(CURDIR) clean

KERNEL_TOP ?= /lib/modules/$(shell uname -r)/build

# define dependency
DEP_DIR ?= $(PWD)/../dbglog
DEP_SYMBOLES ?= 

KBUILD_EXTRA_SYMBOLS += 

# define target object
TARGET_NAME := test-module2
obj-m += $(TARGET_NAME).o
$(TARGET_NAME)-objs += mod2.o ../dbglog/dbglog.o

ccflags-y += -I$(DEP_DIR)


.PHONY: all modules install uninstall test clean

all: modules

modules: 
	make -C $(KERNEL_TOP) M=$(PWD) modules

install: modules
	sudo insmod $(TARGET_NAME).ko

uninstall: 
	@# uninstall order must be reversion of install order.
	sudo rmmod $(TARGET_NAME)

test: 
	make -C $(PWD)/testing M=$(PWD)/testing test

clean:
	# make -C $(DEP_DIR) clean
	# make -C $(PWD)/testing clean
	make -C $(KERNEL_TOP) M=$(PWD) clean

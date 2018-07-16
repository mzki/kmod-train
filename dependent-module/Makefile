KERNEL_TOP ?= /lib/modules/$(shell uname -r)/build

# define dependency
DEP_DIR ?= $(PWD)/dep_timer
DEP_SYMBOLES ?= $(DEP_DIR)/Module.symvers

KBUILD_EXTRA_SYMBOLS += $(DEP_SYMBOLES)

# define target object
TARGET_NAME := dependent_mod
obj-m += $(TARGET_NAME).o

ccflags-y += -I$(DEP_DIR)


.PHONY: all modules install uninstall test clean

all: modules

modules: 
	make -C $(DEP_DIR) modules
	make -C $(KERNEL_TOP) M=$(PWD) modules

install: modules
	make -C $(DEP_DIR) install
	sudo insmod $(TARGET_NAME).ko

uninstall: 
	@# uninstall order must be reversion of install order.
	sudo rmmod $(TARGET_NAME)
	make -C $(DEP_DIR) uninstall

test: 
	make -C $(PWD)/testing M=$(PWD)/testing test

clean:
	make -C $(DEP_DIR) clean
	make -C $(PWD)/testing clean
	make -C $(KERNEL_TOP) M=$(PWD) clean

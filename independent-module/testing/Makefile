KERNEL_TOP ?= /lib/modules/$(shell uname -r)/build

OBJ_NAME := independent_mod_test

obj-m += $(OBJ_NAME).o

ccflags-y += -I../

# color format
BOLD_TEXT=\033[37;1m
OK_TEXT=\033[32;1m
RESET_TEXT=\033[0m

all: modules

modules: 
	make -C $(KERNEL_TOP) M=$(PWD) modules

install:
	sudo insmod $(OBJ_NAME).ko

uninstall:
	sudo rmmod $(OBJ_NAME)

test: modules
	@sudo dmesg -C
	@(sudo insmod $(OBJ_NAME).ko) || { \
		echo "failed to insert module" ;\
		sudo dmesg ;\
		exit 1 ;\
	}
	@sudo rmmod $(OBJ_NAME) # clean up installed module
	@echo "$(BOLD_TEXT)$(OBJ_NAME): $(OK_TEXT)PASS$(RESET_TEXT)"

clean:
	make -C $(KERNEL_TOP) M=$(PWD) clean

obj-m := lakka_character_module.o

KERNEL_DIR := /lib/modules/$(shell uname -r)/build
BUILD_DIR  := $(shell pwd)

all:
	make -C $(KERNEL_DIR) M=$(BUILD_DIR) modules

clean:
	make -C $(KERNEL_DIR) M=$(BUILD_DIR) clean
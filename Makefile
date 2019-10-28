# The check for KERNELRELEASE is used to seperate
# the two parts of the Makefile
# Interestingly, kbuild can see only assignment
# while 'make' can see everything but the assignment
ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
obj-m				 := hi.o

else
# normal makefile
KDIR ?= /lib/modules/`uname -r`/build

default:
				$(MAKE) -C $(KDIR) M=$$PWD
endif

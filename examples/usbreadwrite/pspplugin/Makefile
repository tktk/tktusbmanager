TARGET = usbreadwritetest

OBJS = main.o

USE_KERNEL_LIBC = 1
USE_KERNEL_LIBS = 1

INCDIR = ../../inc
CFLAGS += -std=c11 -Os -G0 -Wall -fno-builtin-printf
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LDFLAGS = -L../../lib
LIBS = -lpspusb_driver -lpspusbbus_driver -ltktusbmanager_driver

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak

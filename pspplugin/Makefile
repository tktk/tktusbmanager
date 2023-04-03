TARGET = tktusbrepeater

OBJS += \
    src/tktusbrepeater/main.o \
    src/tktusbrepeater/config.o \
    parson/parson.o \

USE_KERNEL_LIBC = 1
USE_KERNEL_LIBS = 1

INCDIR = 
CFLAGS += -std=gnu11 -Os -G0 -Wall -fno-builtin-printf -Iinc
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LDFLAGS = 
LIBS = -lpspusb_driver

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak

TARGET = tktusbrepeater

OBJS += \
    src/tktusbrepeater/main.o \
    src/tktusbrepeater/usb/endpoints.o \
    src/tktusbrepeater/usb/endpoint.o \
    src/tktusbrepeater/usb/driver.o \
    src/tktusbrepeater/usb/usb.o \
    src/tktusbrepeater/config/config.o \
    src/tktusbrepeater/common/parson.o \
    src/tktusbrepeater/common/file.o \
    src/tktusbrepeater/common/memory.o \
    parson/parson.o \

USE_KERNEL_LIBC = 1
USE_KERNEL_LIBS = 1

INCDIR = 
CFLAGS += -std=gnu11 -Os -G0 -Wall -fno-builtin-printf -Iinc
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LDFLAGS = 
LIBS = -lc -lpspuser -lpspusb_driver

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak

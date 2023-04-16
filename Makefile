TARGET = tktusbmanager

OBJS += \
    src/tktusbmanager/exports.o \
    src/tktusbmanager/main.o \
    src/tktusbmanager/usb/endpoints.o \
    src/tktusbmanager/usb/endpoint.o \
    src/tktusbmanager/usb/driver.o \
    src/tktusbmanager/usb/usb.o \
    src/tktusbmanager/config/config.o \
    src/tktusbmanager/common/parson.o \
    src/tktusbmanager/common/file.o \
    src/tktusbmanager/common/memory.o \
    parson/parson.o \

USE_KERNEL_LIBC = 1
USE_KERNEL_LIBS = 1

INCDIR = 
CFLAGS += -std=gnu11 -Os -G0 -Wall -fno-builtin-printf -Iinc
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LDFLAGS = 
LIBS = -lc -lpspuser -lpspusb_driver -lpspusbbus_driver

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build_prx.mak

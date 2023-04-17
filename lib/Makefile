TARGET=libtktusbmanager.a
all: $(TARGET)
OBJS = \
    tktusbmanager.o \
    tktusbmanager_tktUsbGetEndpointR.o \
    tktusbmanager_tktUsbGetEndpointW.o \
    tktusbmanager_tktUsbRead.o \
    tktusbmanager_tktUsbWrite.o \

PSPSDK=$(shell psp-config --pspsdk-path)

clean:
	rm -f $(OBJS) $(TARGET)

CC=psp-gcc
INCDIR = 
CFLAGS = -Os -G0 -Wall -fno-builtin-printf -I$(PSPSDK)/include
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)
LDFLAGS=-nodefaultlibs

LIBDIR =

$(TARGET): $(OBJS)
	psp-ar q $(TARGET) $(OBJS)
	psp-ranlib $(TARGET)


$(OBJS): tktusbmanager.S
	$(CC) $(CFLAGS) -DF_$* $< -c -o $@


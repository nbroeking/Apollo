CC=g++
LL=g++
ARMCC=/usr/bin/arm-linux-gnueabi-g++-4.7
ARMLL=/usr/bin/arm-linux-gnueabi-g++-4.7

INCLUDE=  -I../Framework/include \
-I../Librarys/x86/include \
-I../Librarys/x86 \
-I../Librarys/x86/5.3/gcc_64/include \
-I include

LDFLAGS= -Wl,--no-as-needed -lpthread -lpulse -lfftw3 `pkg-config --libs libpulse-simple`
CFLAGS= -Wall -g  -Wl,--no-as-needed $(INCLUDE) `pkg-config --cflags libpulse-simple`

ARMCFLAGS=$(CFLAGS)
ARMLDFLAGS=$(LDFLAGS)

LIBS = -ldl -L tmp -L usr/lib/

export ARMCC
export ARMLL
export ARMCFLAGS
export ARMLDFLAGS

export CC
export LL
export LDFLAGS
export CFLAGS
export LIBS

.PHONY: all clean gen clobber

all:
	$(MAKE) -f raspberrypi.mk

pigen:
	-mkdir obs
	./gen.sh arm
gen:
	-mkdir obs
	./gen.sh host
clean:
	-rm -rf obs
	-rm Manager

clobber:
	-rm -rf obs
	-rm Manager
	-rm raspberrypi.mk

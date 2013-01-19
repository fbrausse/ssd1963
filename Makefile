
CFLAGS += -std=c99 -O -pedantic -Wextra -Wall
#CFLAGS += -mcpu=arm1176jzf-s

SSD_OBJS = ssd1963.o gpio.o ssd-gpio-rpi.o
OBJS = $(SSD_OBJS) ssd-cat.o

.PHONY: all clean distclean

all: ssd-cat

ssd-cat: ssd-cat.o $(SSD_OBJS)

ssd1963.o: CFLAGS += -DSSD_IO_MACROS=\"ssd-gpio.h\"

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) ssd-cat

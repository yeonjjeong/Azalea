# Copyright (c) 1996 - 2001 Caldera International, Inc.
#                  All Rights Reserved.
#
CC=musl-gcc
CCOPT=-O0  -g
LDOPT=-lm -lpthread

KERNEL_DIR = ../../kernel
LIB_DIR = ../../library
ARCH_DIR = ../../include
APP_DIR = ..

INCS = -I$(LIB_DIR) -I$(KERNEL_DIR) -I$(ARCH_DIR)/knl 

CFLAGS= $(CCOPT) $(INCS) 

SRCS =  #futex.c

TASKS = #futex.o

OBJS = $(TASKS) $(LIB_DIR)/*.o

# ugly but portable, even on (very) dumb makes..

.c.o:	futex.h
	$(CC) $(CFLAGS) -c $*.c

all:	futex 
	echo "#!/bin/sh" > true
	chmod +x true

futex: futex.o futex.h $(OBJS)
	$(CC) -static $(CFLAGS) -o futex futex.o $(OBJS) $(LDOPT) 
	cp ./futex ./uapp.elf

futex.o : futex.c futex.h 

clean newrun:
	@echo "Resetting AIM Multiuser Benchmark - Suite VII for another run."
	@rm -f tmp* core *.o uapp.elf futex true

reset:
	@echo "Resetting AIM Multiuser Benchmark - Suite VII to new install condition."
	@rm -f workfile suite.ss logfile results
	@rm -f input futex nohup.out
	@rm -f tmp* core *.o
	@rm -rf fakeh
	@cp Makefile.setup Makefile


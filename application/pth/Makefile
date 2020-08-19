# Copyright (c) 1996 - 2001 Caldera International, Inc.
#                  All Rights Reserved.
#
CC=musl-gcc
CCOPT=-O1  -g
#LDOPT=-lm -lpthread
LDOPT=




KERNEL_DIR = ../../kernel
LIB_DIR = ../../library
ARCH_DIR = ../../include
APP_DIR = ..

#INCS = -I$(LIB_DIR) -I$(KERNEL_DIR) -I$(ARCH_DIR)/$(TARGET)
INCS = -I$(LIB_DIR) -I$(KERNEL_DIR) -I$(ARCH_DIR)/knl
#INCS = -I$(LIB_DIR)

CFLAGS= $(CCOPT) $(INCS) 

SRCS =  #pthread.c

TASKS = #pthread.o

OBJS = $(TASKS) $(LIB_DIR)/*.o

# ugly but portable, even on (very) dumb makes..

.c.o:	pthread.h
	$(CC) $(CFLAGS) -c $*.c

all:	pthread 
	echo "#!/bin/sh" > true
	chmod +x true

pthread: pthread.o pthread.h $(OBJS)
#	$(CC) -static $(CFLAGS) -o pthread pthread.o $(OBJS) $(LDOPT) /usr/local/musl/lib/libpthread.a
	$(CC) -O0 -static $(CFLAGS) -o pthread pthread.o $(OBJS) $(LDOPT) -lpthread
#	$(CC) -static $(CFLAGS) -e main -o pthread pthread.o $(OBJS) $(LDOPT) /usr/local/musl/lib/libpthread.a
#	$(CC) -static $(CFLAGS) -e __libc_start_main -o pthread pthread.o $(OBJS) $(LDOPT) 
	cp ./pthread ./uapp.elf

pthread.o : pthread.c pthread.h 

clean newrun:
	@echo "Resetting AIM Multiuser Benchmark - Suite VII for another run."
	@rm -f tmp* core *.o uapp.elf pthread true

reset:
	@echo "Resetting AIM Multiuser Benchmark - Suite VII to new install condition."
	@rm -f workfile suite.ss logfile results
	@rm -f input pthread nohup.out
	@rm -f tmp* core *.o
	@rm -rf fakeh
	@cp Makefile.setup Makefile


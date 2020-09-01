# Copyright (c) 1996 - 2001 Caldera International, Inc.
#                  All Rights Reserved.
#
CC=musl-gcc
CCOPT=-O1  -g
#LDOPT=-lm -lclone_test
LDOPT=




KERNEL_DIR = ../../kernel
LIB_DIR = ../../library
ARCH_DIR = ../../include
APP_DIR = ..

#INCS = -I$(LIB_DIR) -I$(KERNEL_DIR) -I$(ARCH_DIR)/$(TARGET)
#INCS = -I$(LIB_DIR) -I$(KERNEL_DIR) -I$(ARCH_DIR)/knl
INCS = 
#INCS = -I$(LIB_DIR)

CFLAGS= $(CCOPT) $(INCS) 

SRCS =  #clone_test.c

TASKS = #clone_test.o

OBJS = $(TASKS) $(LIB_DIR)/*.o

# ugly but portable, even on (very) dumb makes..

.c.o:	clone_test.h
	$(CC) $(CFLAGS) -c $*.c

all:	clone_test 
	echo "#!/bin/sh" > true
	chmod +x true

clone_test: clone_test.o clone_test.h $(OBJS)
#	$(CC) -static $(CFLAGS) -o clone_test clone_test.o $(OBJS) $(LDOPT) /usr/local/musl/lib/libclone_test.a
	$(CC) -O0 -static $(CFLAGS) -o clone_test clone_test.o $(OBJS) $(LDOPT) 
#	$(CC) -static $(CFLAGS) -e main -o clone_test clone_test.o $(OBJS) $(LDOPT) /usr/local/musl/lib/libclone_test.a
#	$(CC) -static $(CFLAGS) -e __libc_start_main -o clone_test clone_test.o $(OBJS) $(LDOPT) 
	cp ./clone_test ./uapp.elf

clone_test.o : clone_test.c clone_test.h 

clean newrun:
	@echo "Resetting AIM Multiuser Benchmark - Suite VII for another run."
	@rm -f tmp* core *.o uapp.elf clone_test true

reset:
	@echo "Resetting AIM Multiuser Benchmark - Suite VII to new install condition."
	@rm -f workfile suite.ss logfile results
	@rm -f input clone_test nohup.out
	@rm -f tmp* core *.o
	@rm -rf fakeh
	@cp Makefile.setup Makefile


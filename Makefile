############################################################################
# Makefile for rice encode/decode library and sample program
#
# $Id: Makefile,v 1.3 2008/01/25 07:27:48 michael Exp $
# $Log: Makefile,v $
# Revision 1.3  2008/01/25 07:27:48  michael
# Added CVS log.
#
############################################################################
CC = gcc
LD = gcc
CFLAGS = -O3 -Wall -pedantic -ansi -c
LDFLAGS = -O3 -o

# libraries
LIBS = -L. -lrice -loptlist

# Treat NT and non-NT windows the same
ifeq ($(OS),Windows_NT)
    OS = Windows
endif

ifeq ($(OS),Windows)
    EXE = .exe
    DEL = del
else    #assume Linux/Unix
    EXE =
    DEL = rm
endif

all:        sample$(EXE)

sample$(EXE):   sample.o librice.a liboptlist.a
	$(LD) $^ $(LIBS) $(LDFLAGS) $@

sample.o:   sample.c rice.h optlist.h
	$(CC) $(CFLAGS) $<

librice.a:  rice.o bitfile.o
	ar crv librice.a rice.o bitfile.o
	ranlib librice.a

rice.o: rice.c rice.h bitfile.h
	$(CC) $(CFLAGS) $<

bitfile.o:  bitfile.c bitfile.h
	$(CC) $(CFLAGS) $<

liboptlist.a:   optlist.o
	ar crv liboptlist.a optlist.o
	ranlib liboptlist.a

optlist.o:  optlist.c optlist.h
	$(CC) $(CFLAGS) $<

clean:
	$(DEL) *.o
	$(DEL) *.a
	$(DEL) sample$(EXE)

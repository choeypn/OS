#Nat Choeypant
#3/12/20
#HW 4 Makefile

all:	Simulation

Simulation:	$(DEPEND)
	$(CC) -o Simulation $(CFLAGS) $(FILES)

CC=gcc

CFLAGS=-g -Wall

FILES=Simulation.c

DERIV=${FILES:.c=.o}

DEPEND=$(DERIV)

clean:
	rm -f $(DERIV) Simulation

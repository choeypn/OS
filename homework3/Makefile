#Nat Choeypant
#2/19/20
#Lab 3

all:	Simulation

Simulation:	$(DEPEND)
	$(CC) -o Simulation $(CFLAGS) $(FILES)

CC=gcc

CFLAGS=-g -Wall

FILES=Simulation-1.c

DERIV=${FILES:.c=.o}

DEPEND=$(DERIV)

clean:
	rm -f $(DERIV) Simulation

#============================================================================#
#                SMASH 2014, IEETA/DETI, UNIVERSITY OF AVEIRO                #
#============================================================================#
BIN    = .
CC     = gcc
CPLP   = -fstrict-aliasing -ffast-math -msse2 #-g
#-----------------------------------------------------------------------------
CFLAGS = -O3 -Wall $(CPLP) -DPROGRESS 
#-----------------------------------------------------------------------------
LIBS   = -lm 
DEPS   = defs.h
PROGS  = $(BIN)/smash 
OBJS   = mem.o common.o context.o filters.o segment.o reverse.o paint.o
#-----------------------------------------------------------------------------
all:
	$(MAKE) progs
progs: $(PROGS)
$(BIN)/smash: smash.c $(DEPS) $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN)/smash smash.c $(OBJS) $(LIBS)
mem.o: mem.c mem.h $(DEPS)
	$(CC) -c $(CFLAGS) mem.c
common.o: common.c common.h $(DEPS)
	$(CC) -c $(CFLAGS) common.c
context.o: context.c context.h $(DEPS)
	$(CC) -c $(CFLAGS) context.c
filters.o: filters.c filters.h $(DEPS)
	$(CC) -c $(CFLAGS) filters.c
segment.o: segment.c segment.h $(DEPS)
	$(CC) -c $(CFLAGS) segment.c
reverse.o: reverse.c reverse.h $(DEPS)
	$(CC) -c $(CFLAGS) reverse.c
paint.o: paint.c paint.h $(DEPS)
	$(CC) -c $(CFLAGS) paint.c
clean:
	/bin/rm -f *.o
cleanall:
	/bin/rm -f *.o $(PROGS)
#-----------------------------------------------------------------------------

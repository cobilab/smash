BIN    = .
CC     = gcc
CPLP   = -fstrict-aliasing -ffast-math -msse2
#-----------------------------------------------------------------------------
CFLAGS = -O3 -Wall $(CPLP) #-DPROGRESS
#-----------------------------------------------------------------------------
LIBS   = -lm
DEPS   = defs.h
PROGS  = $(BIN)/smash
OBJS   = mem.o context.o common.o filters.o reverse.o segment.o paint.o 
all:
	$(MAKE) progs
progs: $(PROGS)
$(BIN)/smash: smash.c $(DEPS) $(OBJS)
	$(CC) $(CFLAGS) -o $(BIN)/smash smash.c $(OBJS) $(LIBS)
mem.o: mem.c mem.h $(DEPS)
	$(CC) -c $(CFLAGS) mem.c
context.o: context.c context.h $(DEPS)
	$(CC) -c $(CFLAGS) context.c
common.o: common.c common.h $(DEPS)
	$(CC) -c $(CFLAGS) common.c
filters.o: filters.c filters.h $(DEPS)
	$(CC) -c $(CFLAGS) filters.c
reverse.o: reverse.c reverse.h $(DEPS)
	$(CC) -c $(CFLAGS) reverse.c
segment.o: segment.c segment.h $(DEPS)
	$(CC) -c $(CFLAGS) segment.c
paint.o: paint.c paint.h $(DEPS)
	$(CC) -c $(CFLAGS) paint.c
clean:
	/bin/rm -f *.o
cleanall:
	/bin/rm -f *.o $(PROGS)


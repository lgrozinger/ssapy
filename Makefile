CC=gcc
CFLAGS=-Wall -g -fPIC -I. -Ilib/include
LDFLAGS=-lm -lgsl -lgslcblas
UNITS = depgraph llist priorityq dm nrm ssa
HEADERS = $(patsubst %, lib/include/%.h, $(UNITS))
OBJS = $(patsubst %, build/%.o, $(UNITS)) 
VPATH = lib/src/ lib/include


build/%.o:%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(LDFLAGS)

build : $(OBJS)
	$(CC) -shared -o libssa.so $(OBJS) $(LDFLAGS)

# nrmeg : nrmeg.c
# 	$(CC) $(CFLAGS) -o nrm lib/src/nrmeg.c $(LDFLAGS) -L. -Wl,-rpath=. -lssa

# dmeg : dmeg.c
# 	$(CC) -Wall -g -I. -o dm dmeg.c $(LDFLAGS) -lssa

.PHONY : test
test :
	$(CC) $(CFLAGS) -o check_llist lib/test/check_llist.c lib/src/llist.c -lcheck -lm -lrt -lpthread -lsubunit
	$(CC) $(CFLAGS) -o check_pq lib/test/check_priorityq.c lib/src/priorityq.c lib/src/llist.c -lcheck -lm -lrt -lpthread -lsubunit	
	./check_llist
	./check_pq

.PHONY : clean
clean :
	-rm -r -f build/*.o
	-rm -f libssa.o

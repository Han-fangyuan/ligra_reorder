CC=g++ -std=c++14 -lstdc++ -fopenmp #-fconcepts
# CFLAGS=-O9 -lm

DEBUG?=0 # debug option

EXEC=ord rankedges
MF=makefile # recompile when Makefile has been modified

ifeq ($(DEBUG), 1)
	CFLAGS=-Og -Wextra -g3 -D DEBUG
	o=debug.o
else
	CFLAGS=-Ofast #-Wall # https://cpluspluspedia.com/fr/tutorial/4708/compiler-et-construire
	o=o
endif


UTILS_H =	utils/tools.h \
			utils/inout.h \
			utils/edgelist.h \
			utils/adjlist.h \
			utils/heap.h \
			utils/unitheap.h \
			utils/bfs.h 
			

ORDERS_H = 	order/order_deg.h \
			order/order_rand.h \
			order/order_rcm.h \
			order/order_gorder.h \
			order/order_ldg.h \
			order/order_minla.h \
			order/order_hubsort.h \
			order/order_hubcluster.h \
			order/order_dbg.h \
			order/order_selective_hubsort.h \
			order/order_slashburn.h

UTILS_O =	$(UTILS_H:.h=.$(o))
ORDERS_O = 	$(ORDERS_H:.h=.$(o))

all: $(EXEC)


ord: ord.$(o) $(UTILS_O) $(ORDERS_O)
	$(CC) $^ $(CFLAGS) -o $@

rankedges: rankedges.$(o) $(UTILS_O)
	$(CC) $^ $(CFLAGS) -o $@


ord.$(o): $(UTILS_H) $(ORDERS_H)
rankedges.$(o): $(UTILS_H)



order/order_%.$(o): $(UTILS_H)
order/order_rcm.$(o): order/order_deg.h utils/bfs.h 
order/order_gorder.$(o): order/order_rcm.h
order/order_ldg.$(o): utils/bfs.h 

%.debug.o: %.cpp $(MF)
	$(CC) -c $< $(CFLAGS) -o $@

%.o: %.cpp $(MF)
	@$(CC) -c $< $(CFLAGS) -o $@

.PHONY: clean mrproper

clean:
	rm -rf */*.$(o) *.$(o)

mrproper: clean
	rm -rf $(EXEC)

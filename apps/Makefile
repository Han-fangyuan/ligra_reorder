
#c++14 -fopenmp -march=native -O3 -DOPENMP -DLONG  -DBYTERLE -DVGR  -o PageRank PageRank.C

LONG=1 #vertex is 8 bytes long
EDGELONG= #edge is 4 bytes long
OPENMP=1 #use OPENMP for parallelism
VGR=1 #use VGR formatted graph datasets instead of defult adjacency list

ifdef LONG
INTT = -DLONG
endif

ifdef EDGELONG
INTE = -DEDGELONG
endif

ifdef PD
PD = -DPD
endif

ifdef BYTE
CODE = -DBYTE
else ifdef NIBBLE
CODE = -DNIBBLE
else
CODE = -DBYTERLE
endif

ifdef LOWMEM
MEM = -DLOWMEM
endif

ifdef ALIGNED
ALIGN = -DALIGNED
endif

ifdef VGR
VGR = -DVGR
endif

PCC = g++
PCFLAGS = -std=c++14 -lstdc++ -fopenmp -march=native -O3 -DOPENMP $(INTT) $(INTE) $(CODE) $(PD) $(MEM) $(ALIGN) $(VGR) #-D_OUTPUT_

COMMON_FILES= ligra.h graph.h compressedVertex.h vertex.h utils.h IO.h parallel.h gettime.h timer.h index_map.h maybe.h sequence.h edgeMap_utils.h binary_search.h quickSort.h blockRadixSort.h transpose.h parseCommandLine.h byte.h byteRLE.h nibble.h byte-pd.h byteRLE-pd.h nibble-pd.h vertexSubset.h encoder.C decoder.C pvector.h indicator.h

ALL= BC BFS BellmanFord Components PageRank PageRankDelta Radii 

all: $(ALL)

% : %.C $(COMMON_FILES)
	$(PCC) $(PCFLAGS) -o $@ $<

$(COMMON_FILES):
	ln -s ../ligra/$@ .

.PHONY : clean

clean :
	rm -f *.o $(ALL)

cleansrc :
	rm -f *.o $(ALL)
	rm $(COMMON_FILES)



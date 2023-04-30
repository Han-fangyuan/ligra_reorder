// This code is part of the project "Ligra: A Lightweight Graph Processing
// Framework for Shared Memory", presented at Principles and Practice of
// Parallel Programming, 2013.
// Copyright (c) 2013 Julian Shun and Guy Blelloch
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights (to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <cmath>
#include <sys/mman.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <parallel/algorithm>
#include <omp.h>
#include <cassert>

#include "parallel.h"
#include "blockRadixSort.h"
#include "quickSort.h"
#include "utils.h"
#include "graph.h"
#include "pvector.h"
#include "timer.h"
#include "../apps/sliding_queue.h"

using namespace std;

typedef pair<uintE,uintE> intPair;
typedef pair<uintE, pair<uintE,intE> > intTriple;

template <class E>
struct pairFirstCmp {
  bool operator() (pair<uintE,E> a, pair<uintE,E> b) {
    return a.first < b.first; }
};

template <class E>
struct getFirst {uintE operator() (pair<uintE,E> a) {return a.first;} };

template <class IntType>
struct pairBothCmp {
  bool operator() (pair<uintE,IntType> a, pair<uintE,IntType> b) {
    if (a.first != b.first) return a.first < b.first;
    return a.second < b.second;
  }
};

// A structure that keeps a sequence of strings all allocated from
// the same block of memory
struct words {
  long n; // total number of characters
  char* Chars;  // array storing all strings
  long m; // number of substrings
  char** Strings; // pointers to strings (all should be null terminated)
  words() {}
words(char* C, long nn, char** S, long mm)
: Chars(C), n(nn), Strings(S), m(mm) {}
  void del() {free(Chars); free(Strings);}
};

inline bool isSpace(char c) {
  switch (c)  {
  case '\r':
  case '\t':
  case '\n':
  case 0:
  case ' ' : return true;
  default : return false;
  }
}

_seq<char> mmapStringFromFile(const char *filename) {
  struct stat sb;
  int fd = open(filename, O_RDONLY);
  if (fd == -1) {
    perror("open");
    exit(-1);
  }
  if (fstat(fd, &sb) == -1) {
    perror("fstat");
    exit(-1);
  }
  if (!S_ISREG (sb.st_mode)) {
    perror("not a file\n");
    exit(-1);
  }
  char *p = static_cast<char*>(mmap(0, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0));
  if (p == MAP_FAILED) {
    perror("mmap");
    exit(-1);
  }
  if (close(fd) == -1) {
    perror("close");
    exit(-1);
  }
  size_t n = sb.st_size;
//  char *bytes = newA(char, n);
//  parallel_for(size_t i=0; i<n; i++) {
//    bytes[i] = p[i];
//  }
//  if (munmap(p, sb.st_size) == -1) {
//    perror("munmap");
//    exit(-1);
//  }
//  cout << "mmapped" << endl;
//  free(bytes);
//  exit(0);
  return _seq<char>(p, n);
}

_seq<char> readStringFromFile(char *fileName) {
  ifstream file (fileName, ios::in | ios::binary | ios::ate);
  if (!file.is_open()) {
    std::cout << "Unable to open file: " << fileName << std::endl;
    abort();
  }
  long end = file.tellg();
  file.seekg (0, ios::beg);
  long n = end - file.tellg();
  char* bytes = newA(char,n+1);
  assert(bytes != NULL && "Malloc failure\n");
  file.read (bytes,n);
  file.close();
  return _seq<char>(bytes,n);
}

// parallel code for converting a string to words
words stringToWords(char *Str, long n) {
  {parallel_for (long i=0; i < n; i++)
      if (isSpace(Str[i])) Str[i] = 0; }

  // mark start of words
  bool *FL = newA(bool,n);
  assert(FL != NULL && "Malloc failure\n");
  FL[0] = Str[0];
  {parallel_for (long i=1; i < n; i++) FL[i] = Str[i] && !Str[i-1];}

  // offset for each start of word
  _seq<long> Off = sequence::packIndex<long>(FL, n);
  free(FL);
  long m = Off.n;
  long *offsets = Off.A;

  // pointer to each start of word
  char **SA = newA(char*, m);
  assert(SA != NULL && "Malloc failure\n");
  {parallel_for (long j=0; j < m; j++) SA[j] = Str+offsets[j];}

  free(offsets); 
  return words(Str,n,SA,m);
}

template <class vertex>
graph<vertex> readGraph(char* fname, bool isSymmetric) 
{
#ifndef VGR
	words W;
  
	_seq<char> S = readStringFromFile(fname);
	W = stringToWords(S.A, S.n);

	#ifndef WEIGHTED
	if (W.Strings[0] != (string) "AdjacencyGraph") {
	#else
	if (W.Strings[0] != (string) "WeightedAdjacencyGraph") {
	#endif
	cout << "Bad input file" << endl;
	abort();
	}

	long len = W.m -1;
	long n = atol(W.Strings[1]);
	long m = atol(W.Strings[2]);
	#ifndef WEIGHTED
	if (len != n + m + 2) {
	#else
	if (len != n + 2*m + 2) {
	#endif
	cout << "Bad input file" << endl;
	abort();
	}

	uintT* offsets = newA(uintT,n);
	assert(offsets != NULL && "Malloc failure\n");
	#ifndef WEIGHTED
	uintE* edges = newA(uintE,m);
	#else
	intE* edges = newA(intE,2*m);
	#endif
	assert(edges != NULL && "Malloc failure\n");

	{parallel_for(long i=0; i < n; i++) offsets[i] = atol(W.Strings[i + 3]);}
	{parallel_for(long i=0; i<m; i++) {
	#ifndef WEIGHTED
	  edges[i] = atol(W.Strings[i+n+3]);
	#else
	  edges[2*i] = atol(W.Strings[i+n+3]);
	  edges[2*i+1] = atol(W.Strings[i+n+m+3]);
	#endif
	}}

	//W.del(); // to deal with performance bug in malloc
	W.del(); //The original code ^ commented this out
	#else // #ifdef VGR // Added by Priyank
	if ( sizeof(uintT) != sizeof(unsigned long long) ) {
	  std::cout << sizeof(uintT) << " " << sizeof(unsigned long long) << std::endl;
	  abort();
	}
	if ( sizeof(uintE) != sizeof(unsigned int) ) {
	  std::cout << sizeof(uintE) << " " << sizeof(unsigned int) << std::endl;
	  abort();
	}
	ifstream ifs(fname, std::ios::binary);
	if ( !ifs.good() ) {
	  std::cout << "Unable to open file: " << fname << std::endl;
	  abort();
	}
	unsigned long long major_version;
	unsigned long long minor_version;
	unsigned long long n1;
	unsigned long long m1;
	ifs.read((char*)&major_version, sizeof(unsigned long long));
	ifs.read((char*)&minor_version, sizeof(unsigned long long));
	ifs.read((char*)&n1, sizeof(unsigned long long));
	ifs.read((char*)&m1, sizeof(unsigned long long));

	long n,m;
	m = m1;
	n = n1;
	#ifndef WEIGHTED
	if ( ( major_version != 1 ) || ( minor_version != 0 ) ) {
	  std::cout << "major: " << major_version << " minor: " << minor_version << "  n: " << n << " m: " << m << std::endl;
	  abort();
	}
	#else
	if ( ( major_version != 1 ) || ( minor_version != 4 ) ) {
	  std::cout << "major: " << major_version << " minor: " << minor_version << "  n: " << n << " m: " << m << std::endl;
	  abort();
	}
	#endif

	uintT* offsets = newA(uintT,n);
	assert(offsets != NULL && "Malloc failure\n");
	ifs.read((char*)&offsets[1], sizeof(unsigned long long) * (n-1));
	if ( !ifs || (ifs.gcount() != (sizeof(unsigned long long) * (n-1))) ) {
		std::cout << "Error in reading offsets." << std::endl;
		abort();
	}
	unsigned long long temp;
	ifs.read((char*)&temp, sizeof(unsigned long long) * (1));
	offsets[0] = 0;

	#ifndef WEIGHTED
	uintE* edges = newA(uintE,m);
	#else
	intE* edges_ = newA(intE,2*m);
	assert(edges_ != NULL && "Malloc failure\n");
	intE* edges = newA(intE,2*m);
	#endif
	assert(edges != NULL && "Malloc failure\n");
	#ifndef WEIGHTED
	ifs.read((char*)edges, sizeof(unsigned int) * m);
	if ( !ifs || (ifs.gcount() != (sizeof(unsigned int) * m)) ) {
		std::cout << "Error in reading edges." << std::endl;
		abort();
	}
	#else
	ifs.read((char*)&edges_[0], sizeof(unsigned int) * m);
	if ( !ifs || (ifs.gcount() != (sizeof(unsigned int) * m)) ) {
		std::cout << "Error in reading edges." << std::endl;
		abort();
	}
	if ( (m % 2) == 1 ) {
		unsigned int x = 0;
		ifs.read((char*)&x, sizeof(unsigned int));
		//std::cout << "extra element: " << x << std::endl;
		assert(x == 0);
	}
	ifs.read((char*)&edges_[m], sizeof(unsigned int) * m);
	if ( !ifs || (ifs.gcount() != (sizeof(unsigned int) * m)) ) {
		std::cout << "Error in reading edges." << std::endl;
		abort();
	}
	//std::cout << edges_[m] << std::endl;
	{parallel_for(long i=0; i<m; i++) {
	  edges[2*i] = edges_[i];
	  edges[2*i+1] = edges_[i+m];
	}}
	free(edges_);
	#endif

	ifs.close();
	#endif // VGR

	vertex* v = newA(vertex,n);
	assert(v != NULL && "Malloc failure\n");

	{parallel_for (uintT i=0; i < n; i++) {
	uintT o = offsets[i];
	uintT l = ((i == n-1) ? m : offsets[i+1])-offsets[i];
	v[i].setOutDegree(l);
	#ifndef WEIGHTED
	v[i].setOutNeighbors(edges+o);
	#else
	v[i].setOutNeighbors(edges+2*o);
	#endif
	}}

	if(!isSymmetric) 
	{
	uintT* tOffsets = newA(uintT,n);
	assert(tOffsets != NULL && "Malloc failure\n");
	{parallel_for(long i=0;i<n;i++) tOffsets[i] = INT_T_MAX;}
	#ifndef WEIGHTED
	intPair* temp = newA(intPair,m);
	#else
	intTriple* temp = newA(intTriple,m);
	#endif
	assert(temp != NULL && "Malloc failure\n");
	{parallel_for(long i=0;i<n;i++){
	  uintT o = offsets[i];
	  for(uintT j=0;j<v[i].getOutDegree();j++){
	#ifndef WEIGHTED
	temp[o+j] = make_pair(v[i].getOutNeighbor(j),i);
	#else
	temp[o+j] = make_pair(v[i].getOutNeighbor(j),make_pair(i,v[i].getOutWeight(j)));
	#endif
	  }
	  }}
	free(offsets);

	#ifndef WEIGHTED
	#ifndef LOWMEM
	intSort::iSort(temp,m,n+1,getFirst<uintE>());
	#else
	quickSort(temp,m,pairFirstCmp<uintE>());
	#endif
	#else
	#ifndef LOWMEM
	intSort::iSort(temp,m,n+1,getFirst<intPair>());
	#else
	quickSort(temp,m,pairFirstCmp<intPair>());
	#endif
	#endif

	tOffsets[temp[0].first] = 0;
	#ifndef WEIGHTED
	uintE* inEdges = newA(uintE,m);
	inEdges[0] = temp[0].second;
	#else
	intE* inEdges = newA(intE,2*m);
	inEdges[0] = temp[0].second.first;
	inEdges[1] = temp[0].second.second;
	#endif
	assert(inEdges != NULL && "Malloc failure\n");
	{parallel_for(long i=1;i<m;i++) {
	#ifndef WEIGHTED
	  inEdges[i] = temp[i].second;
	#else
	  inEdges[2*i] = temp[i].second.first;
	  inEdges[2*i+1] = temp[i].second.second;
	#endif
	  if(temp[i].first != temp[i-1].first) {
	tOffsets[temp[i].first] = i;
	  }
	  }}

	free(temp);

	//fill in offsets of degree 0 vertices by taking closest non-zero
	//offset to the right
	sequence::scanIBack(tOffsets,tOffsets,n,minF<uintT>(),(uintT)m);

	{parallel_for(long i=0;i<n;i++){
	  uintT o = tOffsets[i];
	  uintT l = ((i == n-1) ? m : tOffsets[i+1])-tOffsets[i];
	  v[i].setInDegree(l);
	#ifndef WEIGHTED
	  v[i].setInNeighbors(inEdges+o);
	#else
	  v[i].setInNeighbors(inEdges+2*o);
	#endif
	  }}

	free(tOffsets);
	Uncompressed_Mem<vertex>* mem = new Uncompressed_Mem<vertex>(v,n,m,edges,inEdges);
	//std::cout << "Read directed graph. Num Nodes = " << n << " and Num Edges = " << m << "\n";
	return graph<vertex>(v,n,m,mem);
	}
	else {
	free(offsets);
	Uncompressed_Mem<vertex>* mem = new Uncompressed_Mem<vertex>(v,n,m,edges);
	std::cout << "Read undirected graph. Num Nodes = " << n << " and Num Edges = " << m << "\n";
	return graph<vertex>(v,n,m,mem);
	}
}






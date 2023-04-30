#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <fstream>      // ofstream: write, ifstream: read, fstream: read and write from/to files
#include <algorithm>    // std::random_shuffle
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#include <string>
#include <chrono>
#include <cmath>
#include <vector>
#include <utility>
#include <list>

#ifndef DEBUG
#define Debug(msg) {}
#else
#define Debug(msg) { std::cout << "\tDebug: " << msg << endl; fflush(stdout); }
#endif

#define Alert(msg) { std::cout << "\tAlert: " << msg << endl; fflush(stdout); }
#define Info(msg) { std::cout << "\tInfo: " << msg << endl; fflush(stdout); }//强制刷新输出缓冲区

#ifndef NLINKS
#define NLINKS 100000000
#define NNODES 1000000
#endif

#define TimeBegin() auto time_begin = std::chrono::steady_clock::now(); \
                    auto time_previous = time_begin, timeRec_previous = time_begin, time_now = time_begin; \
                    auto clock_begin = std::clock(); \
                    auto clock_previous = clock_begin, clockRec_previous = clock_begin, clock_now = clock_begin;
                  

//记录当前的时间，时钟信息
#define TimeStep(m) std::cout << m; \
                    time_now = std::chrono::steady_clock::now(); \
                    clock_now = std::clock(); \
                    print_time_clock(time_previous, time_now, clock_previous, clock_now); \
                    time_previous = time_now; \
                    clock_previous = clock_now;

#define TimeTotal()	std::cout << "Total"; \
                    time_now = std::chrono::steady_clock::now(); \
                    clock_now = std::clock(); \
                    print_time_clock(time_begin, time_now, clock_begin, clock_now);
                    
//TimeRecStep("tarjan", output)
#define TimeRecStep(m, out) time_now = std::chrono::steady_clock::now(); \
                            clock_now = std::clock(); \
                            out << m << "\t"; \
                            out << std::chrono::duration_cast<std::chrono::milliseconds>(time_now - timeRec_previous).count() << "\t"; \
                            out << clock_now - clockRec_previous << endl; \
                            timeRec_previous = time_now; \
                            clockRec_previous = clock_now;
                            
#define TimeRecTotal(m, out)  time_now = std::chrono::steady_clock::now(); \
                              clock_now = std::clock(); \
                              out << m << "\t"; \
                              out << std::chrono::duration_cast<std::chrono::milliseconds>(time_now - time_begin).count() << "\t"; \
                              out << clock_now - clock_previous << endl;

void print_time_clock(
  std::chrono::steady_clock::time_point begin,
  std::chrono::steady_clock::time_point end,
  std::clock_t cl_begin,
  std::clock_t cl_end);
void print_time(std::chrono::steady_clock::time_point begin, std::chrono::steady_clock::time_point end);

// https://www.cprogramming.com/c++11/c++11-auto-decltype-return-value-after-function.html
// template <typename Builder>
// auto makeAndProcessObject (const Builder& builder) -> decltype( builder.makeObject() );
// template <typename T>
// auto Timer (T &function, int argc, char** argv) -> decltype(function(argc, argv));


typedef unsigned long ul;
typedef unsigned long long ull;
//compute the maximum of three unsigned long
inline ul max3(ul a, ul b, ul c) { return (a >= b and a >= c) ? a : ((b >= c) ? b : c); }

struct Keyvalue 
{
	ul key;
	ul val;
	Keyvalue(ul key, ul val) : key(key), val(val) {}
};

std::vector<ul> order_identity(const ul &n);
std::vector<ul> rank_from_order(const std::vector<ul> &order, const ul &n);
std::vector<ul> rank_from_order(const std::vector<ul> &order);
std::pair<ul,ul> random_pair(const ul &n);

#endif


#ifndef _BENCH_GETTIME_INCLUDED
#define _BENCH_GETTIME_INCLUDED

#include <stdlib.h>
#include <sys/time.h>
#include <iomanip>
#include <iostream>

struct timer 
{
  double totalTime;
  double lastTime;
  double totalWeight;
  bool on;
  struct timezone tzp;
  timer() 
  {
    struct timezone tz = {0, 0};
    totalTime=0.0; 
    totalWeight=0.0;
    on=0; tzp = tz;
  }
  double getTime() 
  {
    timeval now;
    gettimeofday(&now, &tzp);
    return ((double) now.tv_sec) + ((double) now.tv_usec)/1000000.;
  }
  void start () 
  {
    on = 1;
    lastTime = getTime();
  } 
  double stop () 
  {
    on = 0;
    double d = (getTime()-lastTime);
    totalTime += d;
    return d;
  } 
  double stop (double weight) {
    on = 0;
    totalWeight += weight;
    double d = (getTime()-lastTime);
    totalTime += weight*d;
    return d;
  } 
  double total() 
  {
    if (on) return totalTime + getTime() - lastTime;
    else return totalTime;
  }
  double next() 
  {
    if (!on) return 0.0;
    double t = getTime();
    double td = t - lastTime;
    totalTime += td;
    lastTime = t;
    return td;
  }
  //{reportT(next());}
  void reportT(double time) 
  {
    std::cout << std::setprecision(3) << time <<  std::endl;;
  }
  //{reportTime(next());}
  void reportTime(double time) 
  {
    reportT(time);
  }
  void reportStop(double weight, std::string str) 
  {
    std::cout << str << " :" << weight << ": ";
    reportTime(stop(weight));
  }

  // reportTotal()函数
  void reportTotal() 
  {
    double to = (totalWeight > 0.0) ? total()/totalWeight : total();
    reportTime(to);
    totalTime = 0.0;
    totalWeight = 0.0;
  }
  void reportAverageTime(long long rounds)
  {
    double to = (totalWeight > 0.0) ? total()/totalWeight : total();
    double average_time = to/rounds;
    reportTime(average_time);
    totalTime = 0.0;
    totalWeight = 0.0;
  }
  //reportNext()函数
  void reportNext() {reportTime(next());}
  void reportNext(std::string str) {std::cout << str << " : "; reportNext();}
};

static timer _tm;

#define timeStatement(_A,_string) _tm.start();  _A; _tm.reportNext(_string);

#define startTime() _tm.start();
//lastTime = getTime();

#define stopTime(_weight,_str) _tm.reportStop(_weight,_str);

#define reportTime() _tm.reportTotal();
#define reportAverageTime(rounds) _tm.reportAverageTime(rounds);
#define nextTime(_string) _tm.reportNext(_string);
#define click() _tm.next()

#define nextTimeN() _tm.reportT(_tm.next());

#endif 


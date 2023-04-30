// Copyright (c) 2015, The Regents of the University of California (Regents)
// See LICENSE.txt for license details

#ifndef SLIDING_QUEUE_H_
#define SLIDING_QUEUE_H_

#include <algorithm>

#include "platform_atomics.h"


/*
GAP Benchmark Suite
Class:  SlidingQueue
Author: Scott Beamer

Double-buffered queue so appends aren't seen until SlideWindow() called
 - Use QueueBuffer when used in parallel to avoid false sharing by doing
   bulk appends from thread-local storage
*/


template <typename T>
class QueueBuffer;

//滑动队列
//shared_out_start —— shared_out_end —— shared_in
template <typename T>
class SlidingQueue 
{
  T *shared;//共享数组的指针
  size_t shared_in;//享数组中尚未被读取的元素的位置的指针
  size_t shared_out_start;//共享数组中已经被读取的元素的起始位置的指针
  size_t shared_out_end;//共享数组中已经被读取的元素的结束位置的指针
  friend class QueueBuffer<T>;


 public:
  //构造函数
  explicit SlidingQueue(size_t shared_size) {
    shared = new T[shared_size];
    reset();
  }
  //析构函数
  ~SlidingQueue() {
    delete[] shared;
  }
  //push_back
  void push_back(T to_add) {
    shared[shared_in++] = to_add;
  }
  //empty
  bool empty() const {
    return shared_out_start == shared_out_end;
  }
  //reset
  void reset() {
    shared_out_start = 0;
    shared_out_end = 0;
    shared_in = 0;
  }

  void slide_window() {
    shared_out_start = shared_out_end;
    shared_out_end = shared_in;
  }

  typedef T* iterator;

  iterator begin() const {
    return shared + shared_out_start;
  }

  iterator end() const {
    return shared + shared_out_end;
  }

  size_t size() const {
    return end() - begin();
  }
};

// QueueBuffer<uintE> lqueue(queue, numHubs / omp_get_max_threads());
//滑动队列的缓冲区
template <typename T>
class QueueBuffer 
{
  //属性
  size_t in;//无符号整数，当前缓冲区的元素数量
  T *local_queue;//存储本地缓冲区T元素数组的指针
  SlidingQueue<T> &sq;//指向滑动队列对象的引用
  const size_t local_size;//本地缓冲区最大的大小

 public:
  //构造函数
  explicit QueueBuffer(SlidingQueue<T> &master, size_t given_size = 16384)  : sq(master), local_size(given_size) 
  {
    in = 0;
    local_queue = new T[local_size];
  }

  ~QueueBuffer() {
    delete[] local_queue;
  }

  void push_back(T to_add) 
  {
    if (in == local_size)
      flush();
    local_queue[in++] = to_add;
  }

  //将本地缓冲区的元素拷贝到主滑动队列的共享数组中
  void flush() 
  {
    T *shared_queue = sq.shared;
    size_t copy_start = fetch_and_add(sq.shared_in, in);
    std::copy(local_queue, local_queue+in, shared_queue+copy_start);
    in = 0;
  }
};

#endif  // SLIDING_QUEUE_H_

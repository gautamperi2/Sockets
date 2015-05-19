#ifndef CPP11_BLOCKINGQUEUE_H
#define CPP11_BLOCKINGQUEUE_H
///////////////////////////////////////////////////////////////
// Cpp11-BlockingQueue.h - Thread-safe Blocking Queue        //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2013 //
///////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package contains one thread-safe class: BlockingQueue<T>.
 * Its purpose is to support sending messages between threads.
 * It is implemented using C++11 threading constructs including 
 * std::condition_variable and std::mutex.  The underlying storage
 * is provided by the non-thread-safe std::queue<T>.
 *
 * Required Files:
 * ---------------
 * Cpp11-BlockingQueue.h
 *
 * Build Process:
 * --------------
 * devenv Cpp11-BlockingQueue.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 03 Mar 2014
 * - first release
 */

#include <condition_variable>
#include <mutex>
#include <thread>
#include <queue>
#include <string>
#include <iostream>
#include <sstream>

template <typename T>
class BlockingQueue {
public:
  BlockingQueue() {}
  T deQ();
  void enQ(const T& t);
  size_t size();
private:
  BlockingQueue(const BlockingQueue<T>&);
  BlockingQueue<T>& operator=(const BlockingQueue<T>&);
  std::queue<T> q_;
  std::mutex mtx_;
  std::condition_variable cv_;
};

template<typename T>
T BlockingQueue<T>::deQ()
{
  std::unique_lock<std::mutex> l(mtx_);
  if(q_.size() > 0)
  {
    T temp = q_.front();
    q_.pop();
    return temp;
  }
  // may have spurious returns so loop on !condition

  while (q_.size() == 0)
    cv_.wait(l, [this] () { return q_.size() > 0; });
  T temp = q_.front();
  q_.pop();
  return temp;
}

template<typename T>
void BlockingQueue<T>::enQ(const T& t)
{
  {
    std::lock_guard<std::mutex> l(mtx_);
    q_.push(t);
  }
  cv_.notify_one();
}

template<typename T>
size_t BlockingQueue<T>::size()
{
  std::lock_guard<std::mutex> l(mtx_);
  return q_.size();
}

#endif

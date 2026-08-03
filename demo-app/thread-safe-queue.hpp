#ifndef THREAD_SAFE_QUEUE_HPP
#define THREAD_SAFE_QUEUE_HPP

#include <chrono>
#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

namespace demo
{
  template< class T >
  class ThreadSafeQueue
  {
  public:
    ThreadSafeQueue();

    T* front();
    bool empty() const;
    size_t size() const;
    void push(const T& value);
    void push(T&& value);
    void pop();

    void forceStop();
    bool isForceStopped();
  private:
    std::queue< T > queue_;
    mutable std::mutex mutex_;
    std::condition_variable cond_;
    bool force_wait_stop_;

    template< class... Args >
    void emplace_back(Args&&... args);
  };
}

template< class T >
demo::ThreadSafeQueue< T >::ThreadSafeQueue():
  queue_(),
  mutex_(),
  cond_(),
  force_wait_stop_(false)
{}

template< class T >
T* demo::ThreadSafeQueue< T >::front()
{
  std::unique_lock< std::mutex > lock(mutex_);
  cond_.wait(lock, [this] {return !empty() || force_wait_stop_;});
  if (empty())
  {
    return nullptr;
  }
  return std::addressof(queue_.front());
}

template< class T >
bool demo::ThreadSafeQueue< T >::empty() const
{
  std::lock_guard< std::mutex > lock(mutex_);
  return queue_.empty();
}

template< class T >
size_t demo::ThreadSafeQueue< T >::size() const
{
  std::lock_guard< std::mutex > lock(mutex);
  return queue_.size();
}

template< class T >
void demo::ThreadSafeQueue< T >::push(const T& value)
{
  std::lock_guard< std::mutex > lock(mutex_);
  emplace_back(value);
  cond_.notify_one();
}

template< class T >
void demo::ThreadSafeQueue< T >::push(T&& value)
{
  std::lock_guard< std::mutex > lock(mutex_);
  emplace_back(std::forward< T >(value));
  cond_.notify_one();
}

template< class T >
void demo::ThreadSafeQueue< T >::pop()
{
  std::lock_guard< std::mutex > lock(mutex_);
  queue_.pop();
}

template< class T >
void demo::ThreadSafeQueue< T >::forceStop()
{
  force_wait_stop_ = true;
}

template< class T >
bool demo::ThreadSafeQueue< T >::isForceStopped()
{
  return force_wait_stop_ == true;
}

template< class T >
template< class... Args >
void demo::ThreadSafeQueue< T >::emplace_back(Args&&... args)
{
  std::lock_guard< std::mutex > lock(mutex_);
  queue_.emplace(std::forward< Args... >(args...));
}

#endif

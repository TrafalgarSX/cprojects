#ifndef _LOCK_H_
#define _LOCK_H_

#if defined(__clang__) || defined(__GNUC__)
#define _OS_LINUX_
#elif defined(_MSC_VER) && !defined(__clang__)
#define _OS_WINDOWS_  // real windows
#endif

#include <iostream>

#if defined(_OS_LINUX_)
#include <pthread.h>
#elif defined(_OS_WINDOWS_)
#include <windows.h>
#endif

/*
 * 互斥锁
 * 1. 互斥锁是一种最常用的锁，它保证了多线程情况下，共享数据的互斥访问。
 * 2. 互斥锁是一种悲观锁，它保证了共享数据的互斥访问，但是效率较低。
 * pthread
 * 库的锁默认是不可递归的，即同一个线程在未释放锁之前，不能再次对其加锁。
 * windows的 CRITICAL_SECTION Mutex
 * 默认是可递归的，即同一个线程在未释放锁之前，可以再次对其加锁。
 * 所以下面MutexLock在linux和windows下的实现是不一样的。
 * 还没有添加加锁解锁失败的处理
 * 但是LockGuard应该是无法处理加锁失败的情况的，因为它只是在构造函数中加锁，在析构函数中解锁。
 */

class MutexLock {
public:
  MutexLock() {
#if defined(_OS_LINUX_)
    pthread_mutex_init(&mutex_, NULL);
#elif defined(_OS_WINDOWS_)
    InitializeCriticalSection(&mutex_);
#endif
  }
  ~MutexLock() {
#if defined(_OS_LINUX_)
    pthread_mutex_destroy(&mutex_);
#elif defined(_OS_WINDOWS_)
    DeleteCriticalSection(&mutex_);
#endif
  }
  void lock() {
#if defined(_OS_LINUX_)
    pthread_mutex_lock(&mutex_);
#elif defined(_OS_WINDOWS_)
    EnterCriticalSection(&mutex_);
#endif
  }
  void unlock() {
#if defined(_OS_LINUX_)
    pthread_mutex_unlock(&mutex_);
#elif defined(_OS_WINDOWS_)
    LeaveCriticalSection(&mutex_);
#endif
  }

private:
#if defined(_OS_LINUX_)
  pthread_mutex_t mutex_;
#elif defined(_OS_WINDOWS_)
  CRITICAL_SECTION mutex_;
#endif

  // No copying allowed
  MutexLock(const MutexLock &);
  void operator=(const MutexLock &);
};

class MutexLockGuard {
public:
  explicit MutexLockGuard(MutexLock &mutex) : mutex_(mutex) { mutex_.lock(); }
  ~MutexLockGuard() { mutex_.unlock(); }

private:
  MutexLock &mutex_;
  // No copying allowed
  MutexLockGuard(const MutexLockGuard &);
  void operator=(const MutexLockGuard &);
};

class RecursiveMutexLock {
public:
  RecursiveMutexLock() {
#if defined(_OS_LINUX_)
    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&mutex_, &attr);
    pthread_mutexattr_destroy(&attr);
#elif defined(_OS_WINDOWS_)
    mutex_ = CreateMutex(NULL, FALSE, NULL);
#endif
  }
  ~RecursiveMutexLock() {
#if defined(_OS_LINUX_)
    pthread_mutex_destroy(&mutex_);
#elif defined(_OS_WINDOWS_)
    CloseHandle(mutex_);
#endif
  }
  int lock() {
    int ret = 0;
#if defined(_OS_LINUX_)
    ret = pthread_mutex_lock(&mutex_);
    if (ret != 0) {
      std::cout << "pthread_mutex_lock error: " << ret << std::endl;
    }
#elif defined(_OS_WINDOWS_)
    ret = WaitForSingleObject(mutex_, INFINITE);
    if (ret != WAIT_OBJECT_0) {
      std::cout << "WaitForSingleObject error: " << GetLastError() << std::endl;
    }
#endif
    return ret;
  }
  int unlock() {
    int ret = 0;
#if defined(_OS_LINUX_)
    ret = pthread_mutex_unlock(&mutex_);
    if (ret != 0) {
      std::cout << "pthread_mutex_unlock error: " << ret << std::endl;
    }
#elif defined(_OS_WINDOWS_)
    ret = ReleaseMutex(mutex_);
    if (ret == 0) {
      std::cout << "ReleaseMutex error: " << GetLastError() << std::endl;
    }
#endif
    return ret;
  }

private:
#if defined(_OS_LINUX_)
  pthread_mutex_t mutex_;
#elif defined(_OS_WINDOWS_)
  HANDLE mutex_;
#endif

  // No copying allowed
  RecursiveMutexLock(const RecursiveMutexLock &);
  void operator=(const RecursiveMutexLock &);
};

template <typename T>
class RecursiveMutexLockGuard {
public:
  explicit RecursiveMutexLockGuard(T &mutex) : mutex_(mutex) {
    mutex_.lock();
  }
  ~RecursiveMutexLockGuard() { mutex_.unlock(); }

private:
  T &mutex_; // 必须是引用，确保是同一个互斥量
  // No copying allowed
  RecursiveMutexLockGuard(const T &);
  void operator=(const T &);
};

#endif

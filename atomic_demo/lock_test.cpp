#include "lock_test.hpp"
#include <curl/curl.h>
#include <iostream>
#include <string>
#include <thread>

MutexLock mutex;
int number = 0;

void func2() {
  MutexLockGuard lock(mutex);
  std::cout << "number is : " << ++number << std::endl;
}

void func() {
  MutexLockGuard lock(mutex);
  std::cout << "number is : " << ++number << std::endl;
  // func2();
}

int curl_get(char *url) {
  CURL *curl;
  CURLcode res;
  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    // curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
    // curl_easy_setopt(curl, CURLOPT_WRITEDATA, response);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
  }
  return 0;
}

int main(void) {
  int thread_count = 2;
  std::string url("http://www.baidu.com");

  std::cout << "测试 " << std::endl;

#if defined(_OS_LINUX_)
  std::cout << "测试 linux api" << std::endl;
#elif defined(_OS_WINDOWS_)
  std::cout << "测试 windows api" << std::endl;
#endif

#if defined(_LOG_)
  std::cout << "测试 log" << std::endl;
#endif

  std::thread thread_group[10];
  for (int i = 0; i < thread_count; ++i) {
    thread_group[i] = std::thread(func);
    // thread_group[i] = std::thread(curl_get, (char *)url.c_str());
  }

  for (int i = 0; i < thread_count; ++i) {
    thread_group[i].join();
  }

  return 0;
}

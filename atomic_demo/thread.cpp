#include <stdint.h>
#include <windows.h>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex mtx;

thread_local int number = 0;
int global_number = 0;

DWORD tls_index = TLS_OUT_OF_INDEXES;

void thread_function_ref(int &number) {
  mtx.lock();
  number++;
  std::cout << "Hello from thread " << number << std::endl;
  mtx.unlock();
}

void thread_function(int *number) {
  mtx.lock();
  (*number)++;
  std::cout << "Hello from thread " << *number << std::endl;
  // mtx.unlock();
}

void print_index(){
    uint8_t *ptr = (uint8_t *)TlsGetValue(tls_index);
    printf("tls_index: %d, tlsValue: %d\n", tls_index, (*(DWORD*)ptr));
}
void thread_local_function() {
  // cout thread id
#if 1
  number++;
  std::cout << "Hello from thread " << number << std::endl;
#endif

  // Windows get thread id
  DWORD thread_id = GetCurrentThreadId();
  printf("thread id: %d\n", thread_id);

  Sleep(rand() % 1000);
  uint8_t *ptr = new uint8_t[10];
  memset(ptr, 0, 10);
  memcpy(ptr, &thread_id, sizeof(DWORD));
  TlsSetValue(tls_index, ptr);
  // sleep random time 

  print_index();
}

void thread_double_free(void *ptr) {
  printf("thread_double_free: %p\n", ptr);
  free(ptr);
}


/*
 * clang++ -fsanitize=address -fno-omit-frame-pointer -O1 -g thread.cpp -o
 * ./bin/thread.exe 使用ASAN检测内存泄漏, 重复释放, 释放后使用等内存相关问题
 * 添加-g参数可以在ASAN报错时打印出错误的文件和行号,
 * 否则只能打印出错误的地址（需要使用addr2line工具转换）
 * */
int main(void) {
  int number = 0;
  std::thread thread_group[10];
  int thread_num = 5;

  char *ptr = (char *)malloc(10);

  tls_index = TlsAlloc();

  for (int i = 0; i < thread_num; i++) {
    // thread_group[i] = std::thread(thread_function, std::ref(number));
    // thread_group[i] = std::thread(thread_function, &number);
    // thread_group[i] = std::thread(thread_double_free, ptr);
    thread_group[i] = std::thread(thread_local_function);
  }

  for (int i = 0; i < thread_num; i++) {
    thread_group[i].join();
  }

  TlsFree(tls_index);
  return 0;
}

#include <iostream>
#include <vector>

extern "C" void function(int a, int b) {
  std::cout << " extern C function with cpp stl vector body" << std::endl;
  std::vector<int> nums = {0, 1, 2, 3, 4, 5};
  std::cout << "a = " << a << std::endl;
  for (auto num : nums) {
    std::cout << num << std::endl;
  }
}

#if 0
extern "C" void function(char a, int b) {

}
#endif

void test1(int a) { std::cout << "test1" << std::endl; }

void test1(char a) { std::cout << "test2" << std::endl; }

extern "C" void function2(std::vector<int> nums) {
  std::cout << "extern C function parameter with stl vector" << std::endl;
  for (auto num : nums) {
    std::cout << num << std::endl;
  }
}

// void name(std::vector<int> nums);
// extern "C" void name(std::vector<int> nums);
void name(std::vector<int> nums);

int main() {
  // if cpp compiler
#if __cplusplus
  printf("C++\n");
#else
  printf("C\n");
#endif
  std::vector<int> nums = {0, 1, 2, 3, 4, 5};
  int a;
  scanf("%d", &a);
  if (a == 1) {
    goto end;
  }
  function(1, 2);
  function2(nums);
  // int b = 10;
  int b;
  printf("b = %d\n", b);
  name(nums);

end:
  return 0;
}

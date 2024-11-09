#include <iostream>

namespace my_namespace {
extern int global_number;
int static_number;
int my_function(int a, int b);
} // namespace my_namespace

int main(void) {

  std::cout << "global_number is : " << my_namespace::global_number
            << std::endl;
  std::cout << "static_number is : " << my_namespace::static_number
            << std::endl;
  std::cout << "my_function(1, 2) is : " << my_namespace::my_function(1, 2)
            << std::endl;
}

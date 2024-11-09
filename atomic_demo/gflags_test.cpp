#include <iostream>
#include <gflags/gflags.h>
#include <glog/logging.h>

DEFINE_bool(big_menu, false, "List all tests without running them.");
DEFINE_string(languages, "english", "What language to say hello in.");

int main(int argc, char *argv[])
{
  gflags::ParseCommandLineFlags(&argc, &argv, false);

  if (FLAGS_big_menu)
    std::cout << "big_menu is true" << std::endl;
  else 
    std::cout << "big_menu is false" << std::endl;
  std::cout << FLAGS_languages << std::endl;

  return 0;
}


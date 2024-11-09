cmake_minimum_required (VERSION 3.24)
#项目名称
project(test_var)

# 根目录/代码路径

# 添加子目录
include_directories("${PROJECT_SOURCE_DIR}/include")


# 编译成可执行文件
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build) ###### bin，改成build文件，2019.10.17
add_executable(test ${PROJECT_SOURCE_DIR}/src/conflict.c)
add_executable(testpp ${PROJECT_SOURCE_DIR}/src/test.cpp)

# When compiled by using /Za, a jump to the label prevents the identifier from being initialized.

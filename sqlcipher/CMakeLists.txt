cmake_minimum_required(VERSION 3.20) 

# compiler
# set(CMAKE_C_COMPILER "gcc.exe")
# set(CMAKE_CXX_COMPILER "g++.exe")

# set(CMAKE_C_COMPILER "clang.exe")
# set(CMAKE_CXX_COMPILER "clang++.exe")

project(libcurlTest VERSION 1.0.0)

# set(CMAKE_CXX_FLAGS
# 	-std=c++14
# )

#version standard
Set(CMAKE_C_STANDARD 11)
Set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)


#headers
# include_directories(AFTER ${PROJECT_SOURCE_DIR}/include/)
# file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/include/*.h )
# source_group("Header Files" FILES ${HEADERS})
#marco
# add_definitions(-D_DEBUG ) #

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#print some message  eg. variable 
message(CMAKE_CXX_STANDARD="${CMAKE_CXX_STANDARD}")
include(CMakePrintHelpers)
cmake_print_variables(CMAKE_CXX_STANDARD)
cmake_print_variables(PROJECT_BINARY_DIR)  #运行cmake命令的目录 通常为 ${PROJECT_SOURCE_DIR}/build
cmake_print_variables(PROJECT_SOURCE_DIR)  #工程根目录

#static lib output path  windows: .a .lib   unix: .a
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
# 目标链接库的存放位置 unix: .so
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
# windows: .exe .dll   unix: 可执行文件
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin") #

# aux_source_directory(./gtest.cpp MY_TEST)
# add_executable(gtest ${MY_TEST})
# add_executable(gtest_test gtest.cpp)
# 可执行文件
add_executable(misc misc.c)

set(MY_DEV_LIB_PATH "D:/workspace/dev")

#link lib
if(WIN32)
  message("win32")
  set(MY_DEV_LIB_PATH "D:/workspace/dev")
  add_library(gtest STATIC IMPORTED)
  set_target_properties(gtest PROPERTIES IMPORTED_LOCATION "${MY_DEV_LIB_PATH}/debug-lib/libgtest.a")
    # set_target_properties(gtest PROPERTIES IMPORTED_LOCATION "${MY_DEV_LIB_PATH}/lib/libgtest.a")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
  message("${CMAKE_SYSTEM_NAME}")
  message("${CMAKE_SYSTEM_VERSION}")
  message("${CMAKE_ANDROID_ARCH_ABI}")
  message("${CMAKE_ANDROID_NDK}")
  message("${CMAKE_ANDROID_STL_TYPE}")
elseif(UNIX)
  message("unix")
  message("${CMAKE_SYSTEM_NAME}")
  message("${CMAKE_SYSTEM_VERSION}")
  message("${ANDROID_PLATFORM}")
  message("${CMAKE_ANDROID_ARCH_ABI}")
  message("${CMAKE_ANDROID_NDK}")
  message("${CMAKE_ANDROID_STL_TYPE}")
endif()

if(ANDROID_ABI MATCHES "armeabi-v7a")
  message("armeabi-v7a")
  add_library(curl SHARED IMPORTED)
  set_target_properties(curl PROPERTIES IMPORTED_LOCATION "${MY_DEV_LIB_PATH}/android-lib/armeabi-v7a/libcurl.so")

  add_library(crypto SHARED IMPORTED)
  set_target_properties(crypto PROPERTIES IMPORTED_LOCATION "${MY_DEV_LIB_PATH}/android-lib/armeabi-v7a/libcrypto.so")

  add_library(sqlcipher SHARED IMPORTED)
  set_target_properties(sqlcipher PROPERTIES IMPORTED_LOCATION "${MY_DEV_LIB_PATH}/android-lib/armeabi-v7a/libsqlcipher.so")

elseif(ANDROID_ABI MATCHES "arm64-v8a")
  message("arm64-v8a")
  add_library(curl SHARED IMPORTED)
  set_target_properties(curl PROPERTIES IMPORTED_LOCATION "${MY_DEV_LIB_PATH}/android-lib/arm64-v8a/libcurl.so")

  add_library(crypto SHARED IMPORTED)
  set_target_properties(crypto PROPERTIES IMPORTED_LOCATION "${MY_DEV_LIB_PATH}/android-lib/arm64-v8a/libcrypto.so")

  add_library(sqlcipher SHARED IMPORTED)
  set_target_properties(sqlcipher PROPERTIES IMPORTED_LOCATION "${MY_DEV_LIB_PATH}/android-lib/arm64-v8a/libsqlcipher.so")

endif()

add_library(misc_compiler_flags INTERFACE)
target_compile_features(misc_compiler_flags INTERFACE c_std_11)
# add_compile_options(-std=c11)

list(APPEND EXTRA_LIBS curl crypto sqlcipher)

# 眼瞎时刻，这个是库目录，不是头文件目录
# target_link_directories(misc PUBLIC "${MY_DEV_LIB_PATH}/include")
target_include_directories(misc PUBLIC "${MY_DEV_LIB_PATH}/include")

target_link_libraries(misc PUBLIC ${EXTRA_LIBS} misc_compiler_flags)



# target_link_libraries(libcurlTest ws2_32 wldap32 crypt32)
#remind me 删除了 utls里的schannel才通过编译， 如果要与openssl一起编译的话，需要放回来

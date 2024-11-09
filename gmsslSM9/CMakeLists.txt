cmake_minimum_required(VERSION 3.18) 

project(gmsslSM9 VERSION 1.0.0)

message("CMAKE_C_COMPILER=${CMAKE_C_COMPILER}")
message("CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")

#version standard
Set(CMAKE_C_STANDARD 11)
Set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#print some message  eg. variable 
message(CMAKE_CXX_STANDARD="${CMAKE_CXX_STANDARD}")

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  set(ARCH_TYPE "${CMAKE_SYSTEM_PROCESSOR}")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(ARCH_TYPE "${CMAKE_SYSTEM_PROCESSOR}")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
  set(ARCH_TYPE "${CMAKE_ANDROID_ARCH_ABI}")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  message("64bit")
  set(BUILD_BIT "x64")
else()
  # 暂时用不到32位
  message("32bit")
  set(BUILD_BIT "x86")
endif()


#static lib output path  windows: .a .lib   unix: .a
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${ARCH_TYPE}/$<IF:$<CONFIG:Debug>,Debug,Release>/lib")
# 目标链接库的存放位置 unix: .so
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${ARCH_TYPE}/$<IF:$<CONFIG:Debug>,Debug,Release>/lib")
# windows: .exe .dll   unix: 可执行文件
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${ARCH_TYPE}/$<IF:$<CONFIG:Debug>,Debug,Release>/bin")

# 通用宏定义
list(APPEND EXTRA_DEFINES)
# 编译选项 目前只支持gcc和clang, msvc用CMAKE_CXX_FLAGS
list(APPEND EXTRA_COMPILE_FLAGS)
# 依赖的库的路径
list(APPEND EXTRA_LIB_PATH)
# 依赖的库
list(APPEND EXTRA_LIB)

list(APPEND EXTRA_INCLUDE  
                           "${PROJECT_SOURCE_DIR}/include" # 本工程的头文件
                           )

list(APPEND EXTRA_SRC 
              ${PROJECT_SOURCE_DIR}/src/SM9.c
              ${PROJECT_SOURCE_DIR}/src/asn1.c
              ${PROJECT_SOURCE_DIR}/src/debug.c
              ${PROJECT_SOURCE_DIR}/src/hex.c
              ${PROJECT_SOURCE_DIR}/src/rand_win.c
              ${PROJECT_SOURCE_DIR}/src/sm3_hmac.c
              ${PROJECT_SOURCE_DIR}/src/sm3_kdf.c
              ${PROJECT_SOURCE_DIR}/src/sm3.c
              ${PROJECT_SOURCE_DIR}/src/sm9_alg.c
              ${PROJECT_SOURCE_DIR}/src/sm9_key.c
              ${PROJECT_SOURCE_DIR}/src/sm9_lib.c
              ${PROJECT_SOURCE_DIR}/src/gmsslsm9.def
  )

#link lib 后面应该完善，操作系统和编译器组合判断
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  message("windows")
# 根据编译器的不同，设置不同的编译选项和依赖库
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    message("mingw")
    # 方便生成compile_commands.json
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message("clang")
  elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    message("msvc")
    endif()
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  message("unix")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
endif()
message("EXTRA_COMPILE_FLAGS: ${EXTRA_COMPILE_FLAGS}")
message("EXTRA_SRC: ${EXTRA_SRC}")

# 导出所有符号
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_library(common INTERFACE)

# target_link_options(${PROJECT_NAME} PUBLIC ${EXTRA_LINK_FLAGS})
# 编译选项
target_compile_options(common INTERFACE ${EXTRA_COMPILE_FLAGS})
# 宏定义
target_compile_definitions(common INTERFACE ${EXTRA_DEFINES})
# 工程下的头文件的路径
target_include_directories(common INTERFACE ${EXTRA_INCLUDE})
# 第三方库的路径
target_link_directories(common INTERFACE ${EXTRA_LIB_PATH})
# 第三方库
target_link_libraries(common INTERFACE ${EXTRA_LIB})

add_executable(${PROJECT_NAME}test)
add_library(${PROJECT_NAME} SHARED)
add_library(${PROJECT_NAME}s STATIC)

target_link_libraries(${PROJECT_NAME}s PRIVATE common)
target_sources(${PROJECT_NAME}s PRIVATE ${EXTRA_SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE common )
target_sources(${PROJECT_NAME} PRIVATE ${EXTRA_SRC})

target_link_libraries(${PROJECT_NAME}test PRIVATE common ${PROJECT_NAME}s)
# 源文件
target_sources(${PROJECT_NAME}test PUBLIC ${PROJECT_SOURCE_DIR}/src/main.cpp)

add_library(_SM9 SHARED)

if(1)
    target_include_directories(_SM9 PRIVATE c:/python/include ${EXTRA_INCLUDE} ${PROJECT_SOURCE_DIR}/src)
    target_link_directories(_SM9 PRIVATE c:/python/libs)
    target_link_libraries(_SM9 PRIVATE ${PROJECT_NAME})
    target_sources(_SM9 PRIVATE ${PROJECT_SOURCE_DIR}/swig/SM9_wrap.c)

add_custom_command(TARGET _SM9 POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                       $<TARGET_FILE_DIR:_SM9>/${PROJECT_NAME}.dll
                       ${PROJECT_SOURCE_DIR}/swig
                       )

add_custom_command(TARGET _SM9 POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy
                       $<TARGET_FILE_DIR:${PROJECT_NAME}s>/${PROJECT_NAME}.lib
                       ${PROJECT_SOURCE_DIR}/swig
                       )
endif()

include(CTest)

# add_test(NAME first COMMAND gtest_server)
# add_test(NAME second COMMAND gtest_client)

# find_package(SWIG REQUIRED)
# include(UseSWIG)
# # add_library(_SM9 SHARED)
# swig_add_library(_SM9 LANGUAGE python SOURCES 
#                          ${PROJECT_SOURCE_DIR}/swig/SM9.i ${EXTRA_SRC})
# target_include_directories(_SM9 PRIVATE c:/python/include ${EXTRA_INCLUDE} ${PROJECT_SOURCE_DIR}/src)
# target_link_directories(_SM9 PRIVATE c:/python/libs)
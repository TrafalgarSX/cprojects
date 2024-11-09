cmake_minimum_required(VERSION 3.18) 

project(socket_server VERSION 1.0.0)

message("CMAKE_C_COMPILER=${CMAKE_C_COMPILER}")
message("CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}")

set(MY_DEV_LIB_PATH "D:/workspace/dev")
#version standard
Set(CMAKE_C_STANDARD 11)
Set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#print some message  eg. variable 
message(CMAKE_CXX_STANDARD="${CMAKE_CXX_STANDARD}")

#headers 官方不推荐使用GLOB,因为当添加或删除文件时，cmake不会重新生成makefile
file(GLOB HEADERS ${PROJECT_SOURCE_DIR}/include/*.h )
# 可以在visual studio中显示目录结构
source_group("Include" FILES ${HEADERS})

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

# 取消CMake默认生成的工程选项，仅保留Debug与Release（只对msvc这样的多样化构建ide有效）
if(CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_CONFIGURATION_TYPES Release Debug)
  message("CMAKE_CONFIGURATION_TYPES: ${CMAKE_CONFIGURATION_TYPES}")
  set(CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
        "Reset the configurations to what we need"
        FORCE)
endif()

add_executable(${PROJECT_NAME})
add_executable(socket_client)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  # 设置运行时库的类型 MT MD MTd MDd
  # visual studio 目前不能很好的与target_compile_options配合使用，所以这里直接使用CMAKE_CXX_FLAGS
  message("set msvc runtime library type")
  message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
  # set(CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG} /D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")
  # set(CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS_RELEASE} /MT /O2 /Ob2 /D NDEBUG")

  # set_property(TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# 通用宏定义
list(APPEND EXTRA_DEFINES)
# 编译选项 目前只支持gcc和clang, msvc用CMAKE_CXX_FLAGS
list(APPEND EXTRA_COMPILE_FLAGS)
# 依赖的库的路径
list(APPEND EXTRA_LIB_PATH)
# 依赖的库
list(APPEND EXTRA_LIB)

list(APPEND EXTRA_INCLUDE  "${PROJECT_SOURCE_DIR}/include" # 本工程的头文件
                           "${MY_DEV_LIB_PATH}/include" # 第三方库的头文件
                           )

list(APPEND EXTRA_SRC "${PROJECT_SOURCE_DIR}/socket_server.c")

#link lib 后面应该完善，操作系统和编译器组合判断
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  message("windows")
# 根据编译器的不同，设置不同的编译选项和依赖库
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    message("mingw")
    # 方便生成compile_commands.json
    list(APPEND EXTRA_INCLUDE "D:/workspace/dev/vcpkg/installed/${BUILD_BIT}-windows/include")
    list(APPEND EXTRA_LIB_PATH "${MY_DEV_LIB_PATH}/unixlib")
    list(APPEND EXTRA_COMPILE_FLAGS "-Wall")

    list(APPEND EXTRA_LIB gtest pthread)

  elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    message("clang")

  elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    message("msvc")
    # set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_TOOLCHAIN_FILE "D:/workspace/dev/vcpkg/scripts/buildsystems/vcpkg.cmake")
    set(CMAKE_PREFIX_PATH "D:/workspace/dev/vcpkg/installed/${BUILD_BIT}-windows")
    set(VCPKG_FEATURE_FLAGS "versions")

    # 第三方库的头文件（vcpkg安装的）
    list(APPEND EXTRA_INCLUDE "${CMAKE_PREFIX_PATH}/include")
    # 第三方库的路径（vcpkg安装的）
    # list(APPEND EXTRA_LIB_PATH "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>bin")
    # 区分debug和release, 多配置编译器不能直接使用CMAKE_BUILD_TYPE
    list(APPEND EXTRA_LIB_PATH "${CMAKE_PREFIX_PATH}-static/$<$<CONFIG:Debug>:debug/>lib")
    list(APPEND EXTRA_LIB_PATH "${CMAKE_PREFIX_PATH}/$<$<CONFIG:Debug>:debug/>lib")
    # 自己编译的库的路径(msvc编译器)
    list(APPEND EXTRA_LIB_PATH "${MY_DEV_LIB_PATH}/msvclib")

    # list(APPEND EXTRA_DEFINES "_LOG_")
    # list(APPEND EXTRA_COMPILE_FLAGS "$<IF:$<CONFIG:Debug>,MTd,MT>")
    list(APPEND EXTRA_COMPILE_FLAGS "/MD$<$<CONFIG:Debug>:d>")


    # 动态库
    find_package(CURL CONFIG REQUIRED)
    find_package(ZLIB REQUIRED)
    find_package(gflags REQUIRED)
    find_package(glog CONFIG REQUIRED)
    list(APPEND EXTRA_DEFINES "GLOG_NO_ABBREVIATED_SEVERITIES")
    find_package(libuv CONFIG REQUIRED)

    list(APPEND EXTRA_LIB ws2_32.lib)
    # list(APPEND EXTRA_LIB CURL::libcurl ZLIB::ZLIB gflags glog $<IF:$<TARGET_EXISTS:uv_a>,uv_a,uv>)
    # vcpkg 中的gtest 库有点问题，只能用x64-windows-static, 不能用x64-windows
    list(APPEND EXTRA_LIB gtest)

    # 找不到dll时，这样设置就不会提示找不到dll，程序可以正常运行
    set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_PREFIX_PATH}/debug/bin")
  endif()

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  message("unix")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
  list(APPEND EXTRA_LIB_PATH "${MY_DEV_LIB_PATH}/android-lib/${ANDROID_ABI}")
  list(APPEND EXTRA_LIB curl)
endif()
message("EXTRA_COMPILE_FLAGS: ${EXTRA_COMPILE_FLAGS}")
message("EXTRA_SRC: ${EXTRA_SRC}")

# target_link_options(${PROJECT_NAME} PUBLIC ${EXTRA_LINK_FLAGS})
# 编译选项
target_compile_options(${PROJECT_NAME} PUBLIC ${EXTRA_COMPILE_FLAGS})

# 宏定义
target_compile_definitions(${PROJECT_NAME} PUBLIC ${EXTRA_DEFINES})
# 工程下的头文件的路径
target_include_directories(${PROJECT_NAME} PUBLIC ${EXTRA_INCLUDE})
# 第三方库的路径
target_link_directories(${PROJECT_NAME} PUBLIC ${EXTRA_LIB_PATH})
# 第三方库
target_link_libraries(${PROJECT_NAME} PUBLIC ${EXTRA_LIB})
# 源文件
target_sources(${PROJECT_NAME} PUBLIC ${EXTRA_SRC})

# 宏定义
target_compile_definitions(socket_client PUBLIC ${EXTRA_DEFINES})
# 工程下的头文件的路径
target_include_directories(socket_client PUBLIC ${EXTRA_INCLUDE})
# 第三方库的路径
target_link_directories(socket_client PUBLIC ${EXTRA_LIB_PATH})
# 第三方库
target_link_libraries(socket_client PUBLIC ${EXTRA_LIB})
# 源文件
target_sources(socket_client PUBLIC socket_client.c)


add_library(link_test SHARED link_test.c)

add_executable(pointerAdd pointerAdd.c)

add_executable(thread_local thread.cpp)


# enable_testing()

include(CTest)

add_test(NAME first COMMAND gtest_server)
add_test(NAME second COMMAND gtest_client)
# add_test(NAME chinese COMMAND gflags_test --big_menu --languages="zh_CN")
# add_test(NAME en COMMAND gflags_test --languages="chinese")
# add_test(AllTestsInMain main)

# backup for sometime use

# add_library(pthread STATIC IMPORTED)
# set_target_properties(pthread PROPERTIES IMPORTED_LOCATION "${CMAKE_PREFIX_PATH}/lib/pthreadVC3.lib")

# c_flag_overrides.cmake
# if(MSVC)
#   set(CMAKE_C_FLAGS_DEBUG_INIT "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")
#   set(CMAKE_C_FLAGS_MINSIZEREL_INIT     "/MT /O1 /Ob1 /D NDEBUG")
#   set(CMAKE_C_FLAGS_RELEASE_INIT        "/MT /O2 /Ob2 /D NDEBUG")
#   set(CMAKE_C_FLAGS_RELWITHDEBINFO_INIT "/MT /Zi /O2 /Ob1 /D NDEBUG")
# endif()
  # set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd /Od  /Gz /Zi /RTC1 /D_DEBUG ")
  # set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT /O2 /Ob2 /DNDEBUG ")

# aux_source_directory(./gtest.cpp MY_TEST)

# 如何编译32位程序   
# visual studio
# -A x64 -A Win32
# gcc clang 
# add_library(32bit-build INTERFACE)
# target_compile_options(32bit-build INTERFACE -m32)
# target_link_options(32bit-build INTERFACE -m32)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m32")

# 可执行文件后缀
  # set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX "d")
# 库文件后缀
  # set(CMAKE_DEBUG_POSTFIX "d")

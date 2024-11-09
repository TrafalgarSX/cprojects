# $env:CMAKE_MAKE_PROGRAM = ""
# $env:CMAKE_GENERATOR = ""

# 定义参数, 有默认值
param(
  [string]$api_level = "21",
  [string]$abi = "arm64-v8a",
  [string]$stl = "c++_static",
  [string]$build_dir = "build_android"
)

# 简化命令行参数
$api_level = "android-" + "$api_level"

if($abi -eq "arm64-v8a"){
  $build_dir = "$build_dir" + "_arm64"
}else {
  $build_dir = "$build_dir" + "_arm32"
}

echo "--------arguments start-----------"
write-output $api_level
write-output $abi
write-output $stl
write-output $build_dir
echo "--------arguments end-----------"

# 定义变量
$android_ndk_path = $env:NDK # 替换为你的 Android NDK 路径
$android_ndk_path  = $android_ndk_path.Replace("\", "/")
$android_toolchain_file = "$android_ndk_path" + "/build/cmake/android.toolchain.cmake"

# $source_dir = "source"  # 替换为你的源码目录

# 创建构建目录
New-Item -ItemType Directory -Force -Path $build_dir | Out-Null

$android_toolchain_file

# 运行 CMake 命令, 创建一个build目录(与top level CmakeLists.txt同级)
# 在build目录下有不同的构建系统, 如: arm64-v8a, armeabi-v7a, Ninja, make
cmake -S .. -B "$build_dir" -DCMAKE_TOOLCHAIN_FILE="$android_toolchain_file" `
  -DCMAKE_SYSTEM_NAME=Android `
  -DANDROID_NATIVE_API_LEVEL="$api_level" `
  -DANDROID_ABI="$abi" `
  -DANDROID_STL="$stl" `
  -G "Ninja" 


#  CMAKE_HOST_SYSTEM_NAME  Windows
# 编译程序
# cmake --build .

# 编译程序和动态库
# cmake --build $build_dir --target my_program --config Release
# cmake --build $build_dir --target my_library --config Release

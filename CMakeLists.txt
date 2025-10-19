# 指定构建项目所需最低 CMAKE 版本
cmake_minimum_required(VERSION 3.10)
# 需要在定义项目名称之前设置 vcpkg，否则 vcpkg 不会生效， 并且需要删除缓存之后重新生成
set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
# 定义项目名称，版本和使用的编程语言
project(SpaceFighter VERSION 0.1.0 LANGUAGES C CXX)
# 指定 CMake 编译代码使用的标准为 C++20
set(CMAKE_CXX_STANDARD 20)
# 设置必须支持20，如果不支持则会报错
set(CMAKE_CXX_STANDARD_REQUIRED True)
# 设置编译警告选项
if (MSVC)
    add_compile_options(/W4)# 启用较高级别的警告
else ()
    add_compile_options(-Wall -Wextra -Wpedantic)# 启用全面警告
endif ()
# 设置编译输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR})

# 定义一个变量 TARGET， 由项目名称和系统名称组成,eg. SpaceFighter-Windows;
set(TARGET ${PROJECT_NAME}-${CMAKE_SYSTEM_NAME})

# 在 vcpkg 安装列表中查找 SDL2
find_package(SDL2 CONFIG REQUIRED)
# 添加可执行文件
add_executable(${TARGET} main.cpp)
# 链接库
#target_link_libraries(${TARGET}
#        SDL2::SDL2
#        SDL2::SDL2main
#)
# 更新链接库，使用变量来链接 SDL2库，确保在任何平台在都能正确链接所必要的 SDL2库
target_link_libraries(${TARGET}
        ${SDL2_LIBRARIES}
)
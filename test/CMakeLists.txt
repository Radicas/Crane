cmake_minimum_required(VERSION 3.12)

project(CraneTest)

# 查找Google Test库
find_package(GTest REQUIRED)

# 递归查找并包含src文件夹及其子文件夹中的所有.cpp和.h文件
file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/algorithm/geometry/*.cpp" "${CMAKE_SOURCE_DIR}/algorithm/geometry/*.h")
file(GLOB_RECURSE SRC_TEST "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

# 添加测试可执行文件
add_executable(CraneTest
    ${SRC_TEST} # 添加你的测试文件
    ${SRC_FILES} # 包含src文件夹及其子文件夹中的所有文件
)

# 链接Google Test库和其他依赖项
target_link_libraries(CraneTest GTest::GTest GTest::Main)

# 添加测试
include(GoogleTest)
gtest_discover_tests(CraneTest)
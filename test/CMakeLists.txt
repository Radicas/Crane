cmake_minimum_required(VERSION 3.12)

project(CraneTest)

# 查找Google Test库
find_package(GTest REQUIRED)

# 递归查找并包含src文件夹及其子文件夹中的所有.cpp和.h文件
file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/src/algorithm/*.h")
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/src/algorithm/*.cpp")
file(GLOB_RECURSE TEST_SOURCES "${CMAKE_CURRENT_DIR}/src/*.cpp")

# 添加测试可执行文件
add_executable(CraneTest
    ${HEADERS}
    ${SOURCES}
    ${TEST_SOURCES}
)

# 链接Google Test库和其他依赖项
target_link_libraries(CraneTest GTest::GTest GTest::Main)

# 添加测试
include(GoogleTest)
gtest_discover_tests(CraneTest)
# 最低cmake版本
cmake_minimum_required(VERSION 3.5)

# 设置策略，为了cmake版本的兼容性
cmake_policy(SET CMP0074 NEW)

# 设置项目名称
set(PROJECT_NAME Crane)

project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

# 苹果系统需要手动指定qt路径
#if(MACOS)
set(CMAKE_PREFIX_PATH "/usr/local/Cellar/qt@5/5.15.8_2")
#endif()

# 设置Boost库路径
set(BOOST_ROOT "/path/to/boost")

# 设置安装路径
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)

# 子目录
add_subdirectory(src)

# 测试代码
add_subdirectory(test)
add_subdirectory(third_party/gtest)
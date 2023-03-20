# 最低cmake版本
cmake_minimum_required(VERSION 3.5)

# 设置项目名称
set(PROJECT_NAME Scarab)
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

# 苹果系统需要手动指定qt路径
#if(MACOS)
set(CMAKE_PREFIX_PATH "/usr/local/Cellar/qt@5/5.15.8_1")
#endif()

# 设置安装路径
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(third_party/gtest)
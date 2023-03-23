project(geometry)

# 设置C++标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

aux_source_directory(geometry SRC_LIST)

# 生成动态库
add_library(${PROJECT_NAME} SHARED ${SRC_LIST})

# # 生成静态库
# add_library(${PROJECT_NAME}_static STATIC ${SRC_LIST})

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/install/bin)
set(PROJECT_NAME test_geometry)

# 寻找所有目标目录下的源文件，包括.h .cpp .ui等等
aux_source_directory(${CMAKE_SOURCE_DIR}/src/generic PROJECT_SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME} gtest)
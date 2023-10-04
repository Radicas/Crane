# 最低cmake版本
cmake_minimum_required(VERSION 3.5)

# 添加子路径
# add_subdirectory(algorithm)

# 自动添加源文件搜索
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# 自动编译ui文件
set(CMAKE_AUTOUIC ON)

# 自动编译meta object文件
set(CMAKE_AUTOMOC ON)

# 自动编译资源文件
set(CMAKE_AUTORCC ON)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置翻译文件
set(TS_FILES Crane_zh_CN.ts)

# 设置库路径
LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/install/lib)

# 寻找qt包
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

# 寻找Boost库并设置需要链接的库
find_package(Boost REQUIRED COMPONENTS system)

# 寻找所有目标目录下的源文件，包括.h .cpp .ui等等
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} PROJECT_SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/pycheck PROJECT_SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/qtlaboratory PROJECT_SOURCES)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/cranegpt PROJECT_SOURCES)

# 将头文件路径和链接库路径添加到编译选项中
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

# 搜索ui文件
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/res/ui")

# 添加可执行文件
if (${QT_VERSION_MAJOR} GREATER_EQUAL 6)
	qt_add_executable(${PROJECT_NAME}
			MANUAL_FINALIZATION
			${PROJECT_SOURCES}
			)
	qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else ()
	if (ANDROID)
		add_library(${PROJECT_NAME} SHARED
				${PROJECT_SOURCES}
				)
	else ()
		add_executable(${PROJECT_NAME}
				${PROJECT_SOURCES}
				)
	endif ()
	qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif ()

# 链接库
target_link_libraries(${PROJECT_NAME} PRIVATE
		Qt${QT_VERSION_MAJOR}::Widgets # Qt库
		${Boost_LIBRARIES}             # Boost库
		# geometry                       # 几何库
		)

set_target_properties(${PROJECT_NAME} PROPERTIES
		MACOSX_BUNDLE_GUI_IDENTIFIER radica.Crane.com
		MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
		MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
		MACOSX_BUNDLE TRUE
		WIN32_EXECUTABLE TRUE
		)

# 不指定安装路径这里会安装到C盘
install(TARGETS ${PROJECT_NAME}
		BUNDLE DESTINATION .
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

# QT6的特殊处理
if (QT_VERSION_MAJOR EQUAL 6)
	qt_finalize_executable(${PROJECT_NAME})
endif ()


# CMakeList.txt: Lev_PictureManager 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
#如果环境变量中没有声明opencv的路径则需要加上这一条，如果加上了则无所谓
set(OpenCV_DIR C:\\Program Files (x86)\\opencv\\build)
#找到opencv 的 package
find_package(OpenCV REQUIRED)
#引入opencv的头文件
include_directories(${OpenCV_INCLUDE_DIRS})
#导出windows下的.lib静态链接库用以链接符号
set(WINDOWS_EXPORT_ALL_SYMBOLS ON)
# 导出为动态链接库的形式
add_library (Lev_PictureManager SHARED "Lev_PictureManager.cpp" "Lev_PictureManager.h")
#链接到opencv的.lib文件
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Lev_PictureManager PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。

# CMakeList.txt: Lev_Demo 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

include_directories(../Lev_PictureManager)

# 将源代码添加到此项目的可执行文件。
add_executable (Lev_Demo "Lev_Demo.cpp" "Lev_Demo.h")
target_link_directories(Lev_Demo Lev_PictureManager))
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Lev_Demo PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。

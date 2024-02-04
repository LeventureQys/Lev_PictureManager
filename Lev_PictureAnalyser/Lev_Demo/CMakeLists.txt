# CMakeList.txt: Lev_Demo 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#


set(OpenCV_DIR "C:\\Program Files (x86)\\opencv\\build")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(../Lev_PictureAnalyser)
include_directories(../Lev_ModelInfer/include)
include_directories(../Lev_ModelInfer)

# 将源代码添加到此项目的可执行文件。
add_executable (Lev_Demo "Lev_Demo.cpp" "Lev_Demo.h")
target_link_libraries(Lev_Demo ${CMAKE_BINARY_DIR}/Lev_ModelInfer/Release/Lev_ModelInfer.lib)
target_link_libraries(Lev_Demo Lev_PictureAnalyser)
message(STATUS "123123 ${CMAKE_BINARY_DIR}")
#target_link_libraries(Lev_Demo ${CMAKE_CURRENT_BINARY_DIR}/Lev_PictureAnalyser/Lev_ModelInfer/build/Release/Lev_ModelInfer)
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Lev_Demo PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。

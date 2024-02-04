# CMakeList.txt: Lev_PictureAnalyser 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
set(CMAKE_CXX_STANDARD 17)  # 例如，设置为 C++11
set(OpenCV_DIR "C:\\Program Files (x86)\\opencv\\build")
find_package(OpenCV REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_AUTOMOC ON)
include_directories(${Qt5Widgets_INCLUDE_DIRS})
message(STATSU ${OpenCV_INCLUDE_DIRS})
# 将源代码添加到此项目的可执行文件。
add_library (Lev_PictureAnalyser SHARED "Lev_PictureAnalyser.cpp" "Lev_PictureAnalyser.h")
target_link_libraries(${PROJECT_NAME} PRIVATE ${OpenCV_LIBRARIES} Qt5::Core Qt5::Gui)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Lev_PictureAnalyser PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。

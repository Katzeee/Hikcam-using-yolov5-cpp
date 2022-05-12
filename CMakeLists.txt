cmake_minimum_required(VERSION 3.16)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(cam-detection-yolov5 VERSION 1.0 LANGUAGES C CXX)

set(Torch_DIR /home/xac/local/libtorch/share/cmake/Torch)
find_package(OpenCV REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(Torch REQUIRED)

include_directories(${Torch_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS} 3rdparty/hikcamsdk/inc )

add_subdirectory(3rdparty)
add_subdirectory(src)
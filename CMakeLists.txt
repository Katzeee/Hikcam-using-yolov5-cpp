cmake_minimum_required(VERSION 3.16)

project(cam-detection-yolov5 VERSION 1.0 LANGUAGES CXX)

set(Torch_DIR /home/xac/local/libtorch/share/cmake/Torch)
find_package(OpenCV REQUIRED)
find_package(jsoncpp REQUIRED)
find_package(Torch REQUIRED)

include_directories(${Torch_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS} 3rdparty/hikcamsdk/inc src/yolov5)

add_subdirectory(3rdparty)
add_subdirectory(src)
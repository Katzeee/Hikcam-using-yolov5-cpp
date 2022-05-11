cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(cmake_opencv_torch)

set(Torch_DIR /home/xac/local/libtorch/share/cmake/Torch)

find_package(OpenCV REQUIRED)

find_package(Torch REQUIRED)

aux_source_directory(. SOURCE)

include_directories(${Torch_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS} yolov5/inc)
link_directories(/home/xac/local/libtorch/lib)

add_subdirectory(yolov5 yolov5)
add_subdirectory(hikcam hikcam)


add_executable(test ${SOURCE})

#target_link_libraries(test PRIVATE "${TORCH_LIBRARIES}" ${OpenCV_LIBRARIES})
target_link_libraries(test ${TORCH_LIBRARIES} ${OpenCV_LIBRARIES} yolov5 hikcam)



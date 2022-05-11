cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(cmake_opencv_torch)

set(Torch_DIR /home/xac/local/libtorch/share/cmake/Torch)

find_package(OpenCV REQUIRED)
find_package(jsoncpp REQUIRED)

find_package(Torch REQUIRED)

aux_source_directory(./src SOURCE)

include_directories(${Torch_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS} src/yolov5)
link_directories(/home/xac/local/libtorch/lib)

add_subdirectory(src/yolov5 yolov5)
add_subdirectory(hikcam hikcam)
add_subdirectory(src/jsondeal jsondeal)

add_executable(${PROJECT_NAME} ${SOURCE})

#target_link_libraries(test PRIVATE "${TORCH_LIBRARIES}" ${OpenCV_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${TORCH_LIBRARIES} ${OpenCV_LIBRARIES} yolov5 hikcam jsondeal)



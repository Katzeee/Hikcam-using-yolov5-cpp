cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(cmake_opencv_torch)



aux_source_directory(. SOURCE)

include_directories(${Torch_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS} yolov5 ../3rdparty/hikcamsdk/inc)
link_directories(/home/xac/local/libtorch/lib)

add_subdirectory(yolov5 yolov5)
add_subdirectory(hikcam hikcam)
add_subdirectory(jsondeal jsondeal)

add_executable(${PROJECT_NAME} ${SOURCE})

#target_link_libraries(test PRIVATE "${TORCH_LIBRARIES}" ${OpenCV_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${TORCH_LIBRARIES} ${OpenCV_LIBRARIES} yolov5 hikcam jsondeal)



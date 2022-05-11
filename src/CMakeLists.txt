aux_source_directory(. SOURCE)
include_directories(./)

add_subdirectory(utils utils)
add_subdirectory(yolov5 yolov5)
add_subdirectory(hikcam hikcam)

add_executable(${PROJECT_NAME} ${SOURCE})

#target_link_libraries(test PRIVATE "${TORCH_LIBRARIES}" ${OpenCV_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${TORCH_LIBRARIES} ${OpenCV_LIBRARIES} yolov5 hikcam utils)



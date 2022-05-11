aux_source_directory(. SOURCE)


add_subdirectory(yolov5 yolov5)
add_subdirectory(hikcam hikcam)
add_subdirectory(jsondeal jsondeal)

add_executable(${PROJECT_NAME} ${SOURCE})

#target_link_libraries(test PRIVATE "${TORCH_LIBRARIES}" ${OpenCV_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${TORCH_LIBRARIES} ${OpenCV_LIBRARIES} yolov5 hikcam jsondeal)



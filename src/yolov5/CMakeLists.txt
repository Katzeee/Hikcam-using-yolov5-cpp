project(yolov5)

set (SOURCES
    "yolov5.cpp"
    "yolov5.h"
)


add_library(${PROJECT_NAME} ${SOURCES})


target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES} ${TORCH_LIBRARIES} )

target_include_directories(${PROJECT_NAME} PUBLIC .)


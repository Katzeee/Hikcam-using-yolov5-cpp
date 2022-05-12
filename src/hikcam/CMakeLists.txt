project(hikcam)

set (SOURCES
    "hikcam.cpp"
    "hikcam.h"
)


add_library(${PROJECT_NAME} OBJECT ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES} ${TORCH_LIBRARIES} ${hcnetsdk} ${PlayCtrl} ${AudioRender} utils yolov5 )

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR} )
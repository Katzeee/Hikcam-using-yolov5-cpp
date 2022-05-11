project(yolov5)

set (SOURCES
    "yolov5.cpp"
    "yolov5.h"
)


set(Torch_DIR /home/xac/local/libtorch/share/cmake/Torch)

find_package(Torch REQUIRED)

add_library(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${TORCH_LIBRARIES})

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/inc)
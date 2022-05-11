project(hikcam)

set (SOURCES
    "hikcam.cpp"
    "hikcam.h"
)

find_package(OpenCV REQUIRED)
find_package(Torch REQUIRED)

add_library(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES} ${TORCH_LIBRARIES} ${hcnetsdk} ${PlayCtrl} AudioRender jsondeal utools)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
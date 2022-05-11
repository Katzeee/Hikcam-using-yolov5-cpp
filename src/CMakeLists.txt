# set source files
aux_source_directory(. SOURCE)

# set include directories
#include_directories(yolov5/)

add_subdirectory(utils utils)
add_subdirectory(yolov5 yolov5)
add_subdirectory(hikcam hikcam)

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME} PRIVATE ${TORCH_LIBRARIES} ${OpenCV_LIBRARIES} yolov5 hikcam utils)



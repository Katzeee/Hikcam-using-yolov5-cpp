project(utils)

set (SOURCES
    "utils.cpp"
    "utils.h"
)


add_library(${PROJECT_NAME} OBJECT ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC .)

target_link_libraries(${PROJECT_NAME} PUBLIC jsoncpp_object jsoncpp_static)


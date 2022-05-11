project(jsondeal)

set (SOURCES
    "jsondeal.cpp"
    "jsondeal.h"
)

#find_package(jsoncpp REQUIRED)

#include_directories("../../include/jsondeal")

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC .)

target_link_libraries(${PROJECT_NAME} PRIVATE jsoncpp_object)


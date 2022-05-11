project(utools)

set (SOURCES
    "utools.cpp"
    "utools.h"
)


add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC .)


project(jsondeal)


#find_package(jsoncpp REQUIRED)


add_library(${PROJECT_NAME} ./src/jsondeal.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE jsoncpp_object)


target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/inc)
project(hikcam)

set (SOURCES
    "hikcam.cpp"
    "hikcam.h"
)

find_package(OpenCV REQUIRED)
find_package(Torch REQUIRED)

include_directories(../jsondeal ../utools ./inc)
find_library(hcnetsdk libhcnetsdk.so ./lib)
find_library(PlayCtrl libPlayCtrl.so ./lib)

#add_library(AudioRender SHARED IMPORTED)
#set_target_properties(AudioRender PROPERTIES IMPORTED_LOCATION_NOCONFIG ./lib/libAudioRender.so)
find_library(AudioRender libAudioRender.so ./lib)

link_directories(./lib)



add_library(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES} ${TORCH_LIBRARIES} ${hcnetsdk} ${PlayCtrl} AudioRender)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
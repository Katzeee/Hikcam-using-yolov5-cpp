project(hikcam)

find_package(OpenCV REQUIRED)
find_package(Torch REQUIRED)

add_library(${PROJECT_NAME} ./src/hikcam.cpp)
find_library(hcnetsdk libhcnetsdk.so ./lib)
find_library(PlayCtrl libPlayCtrl.so ./lib)
find_library(AudioRender libAudioRender.so ./lib)

link_directories(./lib)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES} ${TORCH_LIBRARIES} ${hcnetsdk} ${PlayCtrl} AudioRender)

target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR}/inc)
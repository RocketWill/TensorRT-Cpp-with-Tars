cmake_minimum_required(VERSION 2.8)

project(dummy-Yolov5HttpServer)

add_definitions(-std=c++11)
add_definitions(-DAPI_EXPORTS)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

option(TARS_MYSQL "option for mysql" ON)
option(TARS_SSL "option for ssl" OFF)
option(TARS_HTTP2 "option for http2" OFF)

# # test
# set(CMAKE_CUDA_COMPILER /usr/local/cuda/bin/nvcc)
# enable_language(CUDA)

if(WIN32)
    include (c:\\tars\\cpp\\makefile\\tars-tools.cmake)
else()
    include (/usr/local/tars/cpp/makefile/tars-tools.cmake)
endif()

####you can: cd build; cmake .. -DTARS_WEB_HOST={your web host}
set(TARS_WEB_HOST "" CACHE STRING "set web host")
IF (TARS_WEB_HOST STREQUAL "")
	set(TARS_WEB_HOST "http://tars.test.com")
ENDIF ()

include_directories(${PROJECT_SOURCE_DIR}/server/include)
include_directories(${PROJECT_SOURCE_DIR}/model/include)

set(CUDA_INC /usr/local/cuda-11.1/targets/x86_64-linux/include)
set(CUDA_LIB /usr/local/cuda/lib64)
include_directories(${CUDA_INC})
link_directories(${CUDA_LIB})

set(MODEL_DIR ${PROJECT_SOURCE_DIR}/model)
set(MODEL_INC_DIR ${MODEL_DIR}/src)
set(SERVER_DIR ${PROJECT_SOURCE_DIR}/server)
set(SERVER_INC_DIR ${SERVER_DIR}/include)
set(SERVER_SRC_DIR ${SERVER_DIR}/src)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
find_package(jsoncpp REQUIRED)
include_directories(${jsoncpp_INCLUDE_DIRS})
include_directories(${MODEL_INC_DIR})

add_subdirectory(${MODEL_DIR})
add_subdirectory(${SERVER_DIR})

add_executable(${PROJECT_NAME} main.cpp ${SERVER_SRC_DIR}/Yolov5HttpServer.cpp ${MODEL_INC_DIR}/inference.cpp)
target_link_libraries(${PROJECT_NAME} http-server yolov5 myplugins ${jsoncpp_LIBRARIES})


# add_executable(test ${PROJECT_SOURCE_DIR}/test.cpp ${MODEL_INC_DIR}/inference.cpp ${SERVER_DIR}/src/ConvertImage.cpp)


# target_link_libraries(test http-server yolov5 myplugins ${jsoncpp_LIBRARIES} ${OpenCV_LIBRARIES})
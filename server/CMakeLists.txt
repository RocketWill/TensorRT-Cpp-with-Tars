cmake_minimum_required(VERSION 2.8)

project(http-server)
add_definitions(-std=c++11)
add_definitions(-DAPI_EXPORTS)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

set(INC_DIR ${PROJECT_SOURCE_DIR}/include)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

file(GLOB SVR_SRCS ${SRC_DIR}/*.cpp)
file(GLOB MODEL_SRCS ${MODEL_INC_DIR}/*.cpp)
include_directories(${INC_DIR})
include_directories(${MODEL_INC_DIR})

add_library(${PROJECT_NAME} SHARED ${SVR_SRCS})
target_link_libraries(${PROJECT_NAME} tarsparse tarsservant tarsutil pthread myplugins ${jsoncpp_LIBRARIES} ${OpenCV_LIBRARIES})

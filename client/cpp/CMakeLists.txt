cmake_minimum_required(VERSION 3.2.0)
project(request-example)

add_definitions(-std=c++11)
add_definitions(-DAPI_EXPORTS)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/src/*.cpp)

find_package(OpenCV REQUIRED)
find_package(cpr REQUIRED)

include_directories(${OpenCV_INCLUDE_DIR})
include_directories(${cpr_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src)

message(${cpr_LIBRARIES})
add_executable(${PROJECT_NAME} example.cpp ${SRCS})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBRARIES} ${cpr_LIBRARIES})
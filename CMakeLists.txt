cmake_minimum_required(VERSION 3.5)

project(untitled2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})
find_package(Boost COMPONENTS filesystem REQUIRED )

INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(untitled2 main.cpp thread_queue.h)
target_link_libraries(untitled2 ${Boost_LIBRARIES} -lpthread ${OpenCV_LIBS})

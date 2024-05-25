cmake_minimum_required(VERSION 3.10)
project(FMM2DProject)

set(CMAKE_CXX_STANDARD 17)

# Enable OpenMP
find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

include_directories(include)
file(GLOB SOURCES "src/*.cpp")

add_executable(FMM2DProject ${SOURCES})

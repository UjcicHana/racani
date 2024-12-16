cmake_minimum_required(VERSION 3.25)
project(racani)

set(CMAKE_CXX_STANDARD 17)

include_directories("C:/freeglut/include")
include_directories("C:/Eigen")
include_directories("C:/glm")

add_executable(racani main.cpp ObjectLoader.h ObjectLoader.cpp BSpline.h BSpline.cpp)

target_link_libraries(racani freeglut opengl32 glu32)
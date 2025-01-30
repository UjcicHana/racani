cmake_minimum_required(VERSION 3.25)
project(racani)

set(CMAKE_CXX_STANDARD 17)

include_directories("include/freeglut/include")
include_directories("include/glm")

add_executable(racani lab3/main.cpp lab3/Bone.cpp lab3/Bone.h lab3/Chain.cpp lab3/Chain.h lab3/CCDAlgorithm.cpp lab3/CCDAlgorithm.h)

target_link_libraries(racani freeglut opengl32 glu32)

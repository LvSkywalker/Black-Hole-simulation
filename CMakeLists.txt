cmake_minimum_required(VERSION 4.0)
project(Black_Hole_simulation)

set(CMAKE_CXX_STANDARD 20)
find_package(glfw3 REQUIRED)


add_executable(Black_Hole_simulation gravity.cpp)

if(APPLE)
    target_link_libraries(Black_Hole_simulation glfw "-framework OpenGL")
else(UNIX)
    target_link_libraries(Black_Hole_simulation glfw GL)
endif()
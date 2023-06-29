cmake_minimum_required(VERSION 3.0)

project(a3)
set(CMAKE_CXX_STANDARD 11)

find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

add_library(viewer src/Ray.cpp  src/Sphere.cpp src/Plane.cpp src/Box.cpp src/Pointsource.cpp src/Scene.cpp)
target_link_libraries(viewer GLEW::GLEW glm::glm OpenGL::GL SDL2::SDL2)


add_executable(main src/main.cpp)
target_link_libraries(main viewer)


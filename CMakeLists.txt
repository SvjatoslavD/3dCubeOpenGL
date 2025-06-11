cmake_minimum_required(VERSION 3.30)
project(3dCubeOpenGL)

set(CMAKE_CXX_STANDARD 20)

add_executable(main
        main.cpp
)

find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(main PRIVATE
        GLEW::GLEW
        SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System
)
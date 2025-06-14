cmake_minimum_required(VERSION 3.30)
project(3dCubeOpenGL)

set(CMAKE_CXX_STANDARD 20)

find_package(SFML COMPONENTS Network Graphics Window Audio System CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(Stb REQUIRED)

set(SOURCE_FILES
        main.cpp
        src/shader.cpp)

set(HEADER_FILES
        ${Stb_INCLUDE_DIR}
        include/shader.h)

add_executable(main ${HEADER_FILES} ${SOURCE_FILES} )

target_link_libraries(main PRIVATE
        GLEW::GLEW
        SFML::Network SFML::Graphics SFML::Window SFML::Audio SFML::System

)
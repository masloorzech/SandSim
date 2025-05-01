cmake_minimum_required(VERSION 3.31)
project(Sand_simulation)

set(CMAKE_CXX_STANDARD 20)
set(SFML_DIR "/usr/local/Cellar/sfml/3.0.0_1/lib/cmake/SFML")
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_executable(Sand_simulation main.cpp)

target_link_libraries(Sand_simulation SFML::Graphics SFML::Window SFML::System)

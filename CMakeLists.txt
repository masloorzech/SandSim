cmake_minimum_required(VERSION 3.30)

project(Sand_simulation)

set(CMAKE_CXX_STANDARD 20)

if(WIN32)
    message(STATUS "Kompilacja na Windows")
    set(SFML_DIR "C:/libs/SFML-3.0.0/lib/cmake/SFML")
    set(SFML_INCLUDE_DIR "C:/libs/SFML-3.0.0/include")
elseif(APPLE)
    message(STATUS "Kompilacja na macOS")
    set(SFML_DIR "/usr/local/Cellar/sfml/3.0.0_1/lib/cmake/SFML")
endif()

find_package(SFML 3 REQUIRED COMPONENTS Graphics Window System)

add_executable(Sand_simulation main.cpp components/Slider.cpp utils/color_utils.cpp components/Button.cpp SandMap.cpp)

target_include_directories(Sand_simulation PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/components
        ${CMAKE_CURRENT_SOURCE_DIR}/utils
        ${CMAKE_CURRENT_SOURCE_DIR}/Tile
)

if(WIN32)
    target_include_directories(Sand_simulation PRIVATE "${SFML_INCLUDE_DIR}")
endif()

target_link_libraries(Sand_simulation SFML::Graphics SFML::Window SFML::System)

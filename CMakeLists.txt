cmake_minimum_required(VERSION 3.5)

project(GameOfLife_SDL2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED CONFIG REQUIRED COMPONENTS SDL2)
find_package(SDL2_image REQUIRED CONFIG COMPONENTS SDL2_image)
find_package(SDL2_ttf REQUIRED CONFIG COMPONENTS SDL2_ttf)
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

add_executable(GameOfLife_SDL2 main.cpp
    elements.h elements.cpp
    field.h field.cpp
    gol.h gol.cpp)

target_link_libraries(GameOfLife_SDL2 PRIVATE SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf ${SDL2_LIBRARIES})

include(GNUInstallDirs)
install(TARGETS GameOfLife_SDL2
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

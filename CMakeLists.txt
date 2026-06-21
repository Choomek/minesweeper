cmake_minimum_required(VERSION 3.16)
project(Saper)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

add_executable(Saper
        main.cpp
        MinesweeperBoard.cpp
        MSBoardTextView.cpp
        MSTextController.cpp
        MSSFMLView.cpp
        MSSFMLController.cpp
        MSSFMLController.h
)

target_link_libraries(Saper
        sfml-graphics
        sfml-window
        sfml-system
)
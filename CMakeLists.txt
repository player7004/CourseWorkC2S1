cmake_minimum_required(VERSION 3.5)

project(CourseWork LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


find_package(Threads REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})

add_executable(CourseWork
            main.cpp
            src/entity.hpp
            src/entity.cpp
            src/window.hpp                
            src/window.cpp
            src/map.cpp
            src/map.hpp
            src/parser.cpp
            src/parser.hpp
)


target_link_libraries(CourseWork PRIVATE Qt5::Widgets ${JSONCPP_LIBRARRIES} Threads::Threads)

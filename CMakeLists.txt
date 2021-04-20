cmake_minimum_required(VERSION 3.5)

project(CourseWork LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
#set(QT_QMAKE_EXECUTABLE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/CourseWork.exe)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

set(Qt5_DIR "C:/Qt/5.15.2/mingw81_64/lib/cmake/Qt5")
set(CMAKE_PREFIX_PATH "C:/Boost/lib/cmake")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O2")

find_package(Threads REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

add_executable(CourseWork
            main.cpp
            src/entity.hpp
            src/entity.cpp
            src/window.hpp                
            src/window.cpp
        src/shopEngine.cpp
        src/shopEngine.hpp
            src/parser.cpp
            src/parser.hpp
            src/dist/jsoncpp.cpp
            src/dist/json/json.h
            src/dist/json/json-forwards.h
)

target_link_libraries(CourseWork PRIVATE Qt5::Widgets Qt5::Core Threads::Threads)

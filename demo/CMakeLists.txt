cmake_minimum_required(VERSION 3.5)

project(demo LANGUAGES CXX)

include(GNUInstallDirs)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick Network REQUIRED)

set(SOURCES
  main.cpp
  democlient.cpp
)

set(HEADERS
  democlient.h
)

set(RESOURCES
  qml.qrc
)

add_executable(demo ${SOURCES} ${HEADERS} ${RESOURCES})

target_include_directories(demo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

target_compile_definitions(demo
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(demo
  PRIVATE Qt5::Core Qt5::Quick Qt5::Network qytlib)

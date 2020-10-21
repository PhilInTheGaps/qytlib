cmake_minimum_required(VERSION 3.4.0)

project(qytlib)

# find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(THIRDPARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty)

# set cmake module path
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(QYTLIB_BUILD_TESTS "Build tests" ON)
option(QYTLIB_BUILD_DEMO "Build demo application" ON)
option(QYTLIB_COVERALLS "Turn on coveralls support" OFF)
option(QYTLIB_COVERALLS_UPLOAD "Upload the generated coveralls json" ON)

include(ide)
include(AddQGumboParser)
include(GNUInstallDirs)
include(CTest)
include(CMakePrintHelpers)

# turn on automatic invocation of the MOC
set(CMAKE_AUTOMOC ON)

if (${CMAKE_VERSION} VERSION_GREATER 3.11)
  add_compile_definitions(QT_DEPRECATED_WARNINGS)
endif()

if (QYTLIB_COVERALLS)
  cmake_print_variables(QYTLIB_COVERALLS_UPLOAD)
	include(coveralls/cmake/Coveralls)
	coveralls_turn_on_coverage()
endif()

find_package(Qt5 REQUIRED COMPONENTS Core Network)

# Try to find tinyxml2,
# if it can not be found, compile from source.
find_library(TINYXML_DIR tinyxml2)
add_subdirectory(thirdparty/tinyxml2)

cmake_print_variables(QYTLIB_BUILD_TESTS)
cmake_print_variables(QYTLIB_BUILD_DEMO)
cmake_print_variables(QYTLIB_COVERALLS)
cmake_print_variables(QYTLIB_COVERALLS_UPLOAD)

set(QYTLIB_SOURCES
  src/common/engagement.cpp
  src/reverseengineering/cipher/reversecipheroperation.cpp
  src/reverseengineering/cipher/slicecipheroperation.cpp
  src/reverseengineering/cipher/swapcipheroperation.cpp
  src/reverseengineering/responses/channelpage.cpp
  src/reverseengineering/responses/closedcaptiontrackresponse.cpp
  src/reverseengineering/responses/dashmanifest.cpp
  src/reverseengineering/responses/embedpage.cpp
  src/reverseengineering/responses/playerconfig.cpp
  src/reverseengineering/responses/playerresponse.cpp
  src/reverseengineering/responses/playersource.cpp
  src/reverseengineering/responses/streaminfovir.cpp
  src/reverseengineering/responses/videoinforesponse.cpp
  src/reverseengineering/responses/watchpage.cpp
  src/reverseengineering/heuristics.cpp
  src/utils/regexutils.cpp
  src/utils/stringutils.cpp
  src/utils/urlutils.cpp
  src/utils/debugutils.cpp
  src/videos/closedcaptions/closedcaptionclient.cpp
  src/videos/closedcaptions/closedcaptionmanifest.cpp
  src/videos/closedcaptions/closedcaptiontrack.cpp
  src/videos/streams/bitrate.cpp
  src/videos/streams/filesize.cpp
  src/videos/streams/streammanifest.cpp
  src/videos/streams/streamsclient.cpp
  src/videos/videoclient.cpp
  src/videos/videoid.cpp
  src/channels/channelid.cpp
  src/channels/username.cpp
  src/channels/channelclient.cpp
)

add_library(qyt ${QYTLIB_SOURCES} ${QYTLIB_HEADERS})

target_include_directories(qyt
  PUBLIC
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/thirdparty/QGumboParser/QGumboParser
)

target_link_libraries(qyt PRIVATE Qt5::Core Qt5::Network qgumboparser tinyxml2)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(
  TARGETS qyt
  EXPORT qyt
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# tests

if (QYTLIB_BUILD_TESTS)
  enable_testing(true)
  add_subdirectory(tests)
endif()

# coveralls report

if (QYTLIB_COVERALLS)
    coveralls_setup(
        "${QYTLIB_SOURCES}"
        ${QYTLIB_COVERALLS_UPLOAD}
        # "${PROJECT_SOURCE_DIR}/coveralls-cmake/cmake"
    )
endif()

# demo application

if (QYTLIB_BUILD_DEMO)
  add_subdirectory(demo)
endif()

cmake_minimum_required(VERSION 3.1.0)

# find includes in the build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# turn on automatic invocation of the MOC
set(CMAKE_AUTOMOC ON)

# compiler definitions
if (${CMAKE_VERSION} VERSION_GREATER 3.11)
  add_compile_definitions(QT_DEPRECATED_WARNINGS)
endif()

# submodule libraries
include(AddQGumboParser)

# qt modules
find_package(Qt5 REQUIRED COMPONENTS Core Gui Quick Qml Multimedia DBus)

# source files
set(SOURCES
  common/engagement.cpp
  reverseengineering/cipher/reversecipheroperation.cpp
  reverseengineering/cipher/slicecipheroperation.cpp
  reverseengineering/cipher/swapcipheroperation.cpp
  reverseengineering/responses/dashmanifest.cpp
  reverseengineering/responses/embedpage.cpp
  reverseengineering/responses/playerconfig.cpp
  reverseengineering/responses/playerresponse.cpp
  reverseengineering/responses/playersource.cpp
  reverseengineering/responses/streaminfovir.cpp
  reverseengineering/responses/videoinforesponse.cpp
  reverseengineering/responses/watchpage.cpp
  reverseengineering/heuristics.cpp
  utils/regexutils.cpp
  utils/stringutils.cpp
  utils/urlutils.cpp
  videos/closedcaptions/closedcaptionclient.cpp
  videos/streams/bitrate.cpp
  videos/streams/filesize.cpp
  videos/streams/streammanifest.cpp
  videos/streams/streamsclient.cpp
  videos/videoclient.cpp
  videos/videoid.cpp
)

# header files
set(HEADERS
  common/engagement.h
  common/thumbnailset.h
  reverseengineering/cipher/icipheroperation.h
  reverseengineering/cipher/reversecipheroperation.h
  reverseengineering/cipher/slicecipheroperation.h
  reverseengineering/cipher/swapcipheroperation.h
  reverseengineering/responses/dashmanifest.h
  reverseengineering/responses/embedpage.h
  reverseengineering/responses/istreaminfoprovider.h
  reverseengineering/responses/playerconfig.h
  reverseengineering/responses/playerresponse.h
  reverseengineering/responses/playersource.h
  reverseengineering/responses/streaminfovir.h
  reverseengineering/responses/videoinforesponse.h
  reverseengineering/responses/watchpage.h
  reverseengineering/heuristics.h
  reverseengineering/responses/iresponse.h
  utils/regexutils.h
  utils/stringutils.h
  utils/urlutils.h
  videos/closedcaptions/closedcaptionclient.h
  videos/closedcaptions/iclosedcaptiontrackinfo.h
  videos/streams/bitrate.h
  videos/streams/container.h
  videos/streams/filesize.h
  videos/streams/istreaminfo.h
  videos/streams/streammanifest.h
  videos/streams/streamsclient.h
  videos/video.h
  videos/videoclient.h
  videos/videoid.h
  videos/streams/audioonlystreaminfo.h
  videos/streams/iaudiostreaminfo.h
  videos/streams/ivideostreaminfo.h
  videos/streams/muxedstreaminfo.h
  videos/streams/videoonlystreaminfo.h
)

# target executable
add_library(qytlib STATIC ${SOURCES} ${HEADERS})

target_include_directories(qytlib PUBLIC
  ${CMAKE_SOURCE_DIR}/thirdparty/QGumboParser/QGumboParser
)

target_link_libraries(qytlib Qt5::Core Qt5::Network qgumboparser)

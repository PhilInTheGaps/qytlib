cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(GUMBO_DIR ${CMAKE_CURRENT_LIST_DIR}/../thirdparty/QGumboParser/QGumboParser) 

set(CMAKE_AUTOMOC ON)

enable_testing(true)

find_package(Qt5 REQUIRED COMPONENTS Core Test)

set(SOURCES
  ${GUMBO_DIR}/qgumboattribute.cpp
  ${GUMBO_DIR}/qgumbodocument.cpp 
  ${GUMBO_DIR}/qgumbonode.cpp 
  ${GUMBO_DIR}/gumbo-parser/src/attribute.c 
  ${GUMBO_DIR}/gumbo-parser/src/char_ref.c 
  ${GUMBO_DIR}/gumbo-parser/src/error.c 
  ${GUMBO_DIR}/gumbo-parser/src/parser.c 
  ${GUMBO_DIR}/gumbo-parser/src/string_buffer.c 
  ${GUMBO_DIR}/gumbo-parser/src/string_piece.c 
  ${GUMBO_DIR}/gumbo-parser/src/tag.c 
  ${GUMBO_DIR}/gumbo-parser/src/tokenizer.c 
  ${GUMBO_DIR}/gumbo-parser/src/utf8.c 
  ${GUMBO_DIR}/gumbo-parser/src/util.c 
  ${GUMBO_DIR}/gumbo-parser/src/vector.c
)

set(HEADERS
  ${GUMBO_DIR}/HtmlTag.h
  ${GUMBO_DIR}/qgumboattribute.h
  ${GUMBO_DIR}/qgumbodocument.h 
  ${GUMBO_DIR}/qgumbonode.h
  ${GUMBO_DIR}/gumbo-parser/src/attribute.h 
  ${GUMBO_DIR}/gumbo-parser/src/char_ref.h 
  ${GUMBO_DIR}/gumbo-parser/src/error.h 
  ${GUMBO_DIR}/gumbo-parser/src/gumbo.h 
  ${GUMBO_DIR}/gumbo-parser/src/insertion_mode.h 
  ${GUMBO_DIR}/gumbo-parser/src/parser.h 
  ${GUMBO_DIR}/gumbo-parser/src/string_buffer.h 
  ${GUMBO_DIR}/gumbo-parser/src/string_piece.h 
  ${GUMBO_DIR}/gumbo-parser/src/token_type.h 
  ${GUMBO_DIR}/gumbo-parser/src/tokenizer_states.h 
  ${GUMBO_DIR}/gumbo-parser/src/tokenizer.h 
  ${GUMBO_DIR}/gumbo-parser/src/utf8.h 
  ${GUMBO_DIR}/gumbo-parser/src/util.h 
  ${GUMBO_DIR}/gumbo-parser/src/vector.h 
)

# library
add_library(qgumboparser STATIC ${SOURCES} ${HEADERS})

set_target_properties(qgumboparser PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(qgumboparser Qt5::Core)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_include_directories(qgumboparser PUBLIC ${GUMBO_DIR}/gumbo-parser/visualc/include)
endif()

# tests

if (QYTLIB_BUILD_TESTS)
  enable_testing(true)
  add_executable(test_qgumboparser
    ${GUMBO_DIR}/../tests/tst_qgumboparsertest.h
    ${GUMBO_DIR}/../tests/tst_qgumboparsertest.cpp
  )

  target_include_directories(test_qgumboparser PRIVATE ${CMAKE_SOURCE_DIR}/src)
  target_link_libraries(test_qgumboparser qgumboparser Qt5::Test)
  add_test(NAME test_qgumboparser COMMAND test_qgumboparser)
endif()



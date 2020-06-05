include(CMakePrintHelpers)

if (NOT TINYXML_DIR)
  message(STATUS "TinyXml2 not found, building from source ...")
  
  set(BUILD_SHARED_LIBS OFF CACHE BOOL "Disable shared lib build")
  set(BUILD_STATIC_LIBS ON CACHE BOOL "Enable static lib build")

  add_subdirectory(tinyxml2)
else()
  cmake_print_variables(TINYXML_DIR)
endif()
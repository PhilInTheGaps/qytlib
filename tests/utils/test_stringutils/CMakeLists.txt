find_package(Qt5Test REQUIRED)

add_executable(test_stringutils tst_stringutils.h tst_stringutils.cpp)
target_include_directories(test_stringutils PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_stringutils qyt Qt5::Test)
add_test(NAME test_stringutils COMMAND test_stringutils)

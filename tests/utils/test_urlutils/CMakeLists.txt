find_package(Qt5Test REQUIRED)

add_executable(test_urlutils tst_urlutils.h tst_urlutils.cpp)
target_include_directories(test_urlutils PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_urlutils qytlib Qt5::Test)
add_test(NAME test_urlutils COMMAND test_urlutils)
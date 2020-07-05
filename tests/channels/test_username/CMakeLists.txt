find_package(Qt5Test REQUIRED)

add_executable(test_username tst_username.h tst_username.cpp)
target_include_directories(test_username PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_username qytlib Qt5::Test)
add_test(NAME test_username COMMAND test_username)

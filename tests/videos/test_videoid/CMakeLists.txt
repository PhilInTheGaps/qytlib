find_package(Qt5Test REQUIRED)

add_executable(test_videoid tst_videoid.h tst_videoid.cpp)
target_include_directories(test_videoid PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_videoid qytlib Qt5::Test)
add_test(NAME test_videoid COMMAND test_videoid)
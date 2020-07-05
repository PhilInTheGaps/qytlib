find_package(Qt5Test REQUIRED)

add_executable(test_channelid tst_channelid.h tst_channelid.cpp)
target_include_directories(test_channelid PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_channelid qytlib Qt5::Test)
add_test(NAME test_channelid COMMAND test_channelid)

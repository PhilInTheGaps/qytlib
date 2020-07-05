find_package(Qt5 COMPONENTS Core Network Test REQUIRED)

add_executable(test_channelclient tst_channelclient.h tst_channelclient.cpp)
target_include_directories(test_channelclient PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_channelclient qytlib Qt5::Core Qt5::Network Qt5::Test)
add_test(NAME test_channelclient COMMAND test_channelclient)

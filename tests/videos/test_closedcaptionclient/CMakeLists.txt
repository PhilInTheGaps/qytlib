find_package(Qt5 COMPONENTS Core Network Test REQUIRED)

add_executable(test_closedcaptionclient tst_closedcaptionclient.h tst_closedcaptionclient.cpp)
target_include_directories(test_closedcaptionclient PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_closedcaptionclient qytlib Qt5::Core Qt5::Network Qt5::Test)
add_test(NAME test_closedcaptionclient COMMAND test_closedcaptionclient)
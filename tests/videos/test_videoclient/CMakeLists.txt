find_package(Qt5 COMPONENTS Core Network Test REQUIRED)

add_executable(test_videoclient tst_videoclient.h tst_videoclient.cpp)
target_include_directories(test_videoclient PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_videoclient qytlib Qt5::Core Qt5::Network Qt5::Test)
add_test(NAME test_videoclient COMMAND test_videoclient)
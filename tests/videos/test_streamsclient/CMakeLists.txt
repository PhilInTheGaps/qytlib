find_package(Qt5 COMPONENTS Core Network Test REQUIRED)

add_executable(test_streamsclient tst_streamsclient.h tst_streamsclient.cpp)
target_include_directories(test_streamsclient PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_streamsclient qyt Qt5::Core Qt5::Network Qt5::Test)
add_test(NAME test_streamsclient COMMAND test_streamsclient)

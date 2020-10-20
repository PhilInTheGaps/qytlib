find_package(Qt5Test REQUIRED)

add_executable(test_cipheroperations tst_cipher.h tst_cipher.cpp)
target_include_directories(test_cipheroperations PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(test_cipheroperations qyt Qt5::Test)
add_test(NAME test_cipheroperations COMMAND test_cipheroperations)

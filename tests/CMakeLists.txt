# Unit tests are written using the Boost unit test framework
find_package(Boost REQUIRED COMPONENTS unit_test_framework)

# Find all the binary files used for testing and copy them into the build
# directory. This allows the test to be run from the build directory

# Lastly, copy the script to run the tests
configure_file(run_tests.sh ${CMAKE_CURRENT_BINARY_DIR}/run_tests.sh COPYONLY)

add_executable(
    decenc_test
    test1.cpp test2.cpp test3.cpp)

target_include_directories(decenc_test PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(
    decenc_test
    PRIVATE
        decenc::decenc
        Boost::unit_test_framework)

add_test(
    NAME
        decenc_test
    COMMAND
        ${CMAKE_CURRENT_BINARY_DIR}/decenc_test
    WORKING_DIRECTORY
        ${CMAKE_CURRENT_BINARY_DIR})

add_dependencies(check decenc_test)

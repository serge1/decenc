cmake_minimum_required(VERSION 3.12)

# Detect if we are the top level CMakeLists.txt or are we included in some
# other project
if(NOT DEFINED PROJECT_NAME)
  set(IS_TOP_PROJECT TRUE)
endif()

# Turn this on in order to build decenc examples
option(DECENC_BUILD_EXAMPLES "Build DECENC examples" OFF)

# Turns this on in order to build tests
option(DECENC_BUILD_TESTS    "Build DECENC tests" OFF)

# Read version from header file
set(VERSION_HEADER "include/decenc/decenc_version.hpp")
file(READ ${VERSION_HEADER} ver)
string(REGEX MATCH "#define DECENC_VERSION \"([0-9\.]+)\"" _ ${ver})
if (NOT CMAKE_MATCH_1)
    message(FATAL_ERROR "Unable to parse version from ${VERSION_HEADER}")
endif()
set(VERSION ${CMAKE_MATCH_1})

# Use configure_file to make configure step depend on decenc_version.hpp
configure_file(${VERSION_HEADER} ${CMAKE_CURRENT_BINARY_DIR}/decenc_version.hpp.copy COPYONLY)

project(decenc VERSION ${VERSION} LANGUAGES C CXX)

include(GNUInstallDirs)

# Create a header only CMake target for decenc
add_library(decenc INTERFACE)
add_library(decenc::decenc ALIAS decenc)

target_include_directories(
    decenc
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

if (DECENC_BUILD_EXAMPLES)
    add_subdirectory(src)
endif()

if (DECENC_BUILD_TESTS AND IS_TOP_PROJECT)
    enable_testing()
    add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} USES_TERMINAL)
    add_subdirectory(tests)
endif()

# If this is the top level project, add in logic to install decenc
if (IS_TOP_PROJECT)
    include(CMakePackageConfigHelpers)

    # Create a file that includes the current project version. This will be
    # installed with the decenc CMake package.
    write_basic_package_version_file(
        "${PROJECT_NAME}ConfigVersion.cmake"
        VERSION
            ${PROJECT_VERSION}
        COMPATIBILITY
            SameMajorVersion)
    
    # Create the default ${PROJECT_NAME}Config.cmake file which will be
    # installed and found by calls to `find_package(decenc)`.
    configure_package_config_file(
        "${PROJECT_SOURCE_DIR}/cmake/${PROJECT_NAME}Config.cmake.in"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
        INSTALL_DESTINATION
            ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

    # Install the previously generated "config" and "version" files
    install(
        FILES
            "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
            "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION
            ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

    # Install the entire local `decenc` directory to the include directory
    install(
        DIRECTORY
            decenc
        DESTINATION
            ${CMAKE_INSTALL_INCLUDEDIR})

    # Create a ${PROJECT_NAME}Targets.cmake file that is referenced by the
    # ${PROJECT_NAME}Config.cmake file and includes the target information
    # needed to compile/link against all targets exported under the
    # ${PROJECT_NAME}_Targets export
    install(
        EXPORT
            ${PROJECT_NAME}_Targets
        FILE
            ${PROJECT_NAME}Targets.cmake
        NAMESPACE
            ${PROJECT_NAME}::
        DESTINATION
            ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake)

    # Add the decenc target to the ${PROJECT_NAME}_Targets export
    install(
        TARGETS
            decenc
        EXPORT
            ${PROJECT_NAME}_Targets)
endif()


# Mandatory line, sets the minimum version of CMake that should be used with this repository.
# I specified 3.22 because I trust it. However, currently I have 3.26 installed on my machine.
# To verify your version run
# $ cmake --version
cmake_minimum_required(VERSION 3.29)
add_definitions(-w)

# Sets a few variables, like PROJECT_NAME
project(BlackWatchStudio)

# Always use the newest C++ standard on green-field projects if possible.
set(CMAKE_CXX_STANDARD 23)
if (APPLE)
    # On Mac, we need to wait for a new JUCE version that fixes the compilation issue
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_OSX_DEPLOYMENT_TARGET "11" CACHE STRING "Minimum OS X deployment target" FORCE)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

#if (APPLE)
#    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE INTERNAL "")
#endif()
#set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Developer ID Application: All Star Apps, LLC (768Z86F8ET)")
#set(CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "768Z86F8ET")

# I like to download the dependencies to the same folder as the project.
# If you want to install them system wide, set CPM_SOURCE_CACHE with the path to the dependencies
# either as an environment variable or pass it to the cmake script with -DCPM_SOURCE_CACHE=<path>.
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)

# Downloads CPM if not already downloaded. CPM is an easy-to-use package manager nicely integrated with CMake.
include(cmake/cpm.cmake)

# This commands downloads AND configures JUCE. It sets up some variables, like JUCE_SOURCE_DIR.
CPMAddPackage(
    NAME JUCE
    GIT_TAG 8.0.3
    VERSION 8.0.3
    GITHUB_REPOSITORY juce-framework/JUCE
    SOURCE_DIR ${LIB_DIR}/juce
)

CPMAddPackage(
  NAME nlohmann_json
  GIT_TAG v3.11.2
  GITHUB_REPOSITORY nlohmann/json
  SOURCE_DIR ${LIB_DIR}/nlohmann_json
)

CPMAddPackage(
  NAME httplib
  GIT_TAG v0.12.3
  GITHUB_REPOSITORY yhirose/cpp-httplib
  SOURCE_DIR ${LIB_DIR}/httplib
  OPTIONS
    "BROTLI_USE_STATIC_LIBS ON"
    "OPENSSL_USE_STATIC_LIBS ON"
    "HTTPLIB_USE_BROTLI_IF_AVAILABLE OFF"
)

# Adds all the targets configured in the "plugin" folder.
add_subdirectory(plugin)

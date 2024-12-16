set(CPM_SOURCE_CACHE ${CMAKE_SOURCE_DIR}/libs)
include(${CMAKE_SOURCE_DIR}/cmake/get_cpm.cmake)

# JUCE
CPMAddPackage(
    NAME JUCE
    GIT_TAG 8.0.3
    GITHUB_REPOSITORY juce-framework/JUCE
    SOURCE_DIR ${LIB_DIR}/juce
    OPTIONS
      "JUCE_BUILD_EXTRAS=ON"
)

# nlohmann_json
CPMAddPackage(
  NAME nlohmann_json
  GIT_TAG v3.11.3
  GITHUB_REPOSITORY nlohmann/json
  SOURCE_DIR ${LIB_DIR}/nlohmann_json
)

# httplib
CPMAddPackage(
  NAME httplib
  GIT_TAG v0.18.3
  GITHUB_REPOSITORY yhirose/cpp-httplib
  SOURCE_DIR ${LIB_DIR}/httplib
  OPTIONS
    "BROTLI_USE_STATIC_LIBS ON"
    "OPENSSL_USE_STATIC_LIBS ON"
    "HTTPLIB_USE_BROTLI_IF_AVAILABLE OFF"
)
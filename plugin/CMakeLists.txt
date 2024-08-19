cmake_minimum_required(VERSION 3.24.1)

# Version is needed by JUCE.
project(BlackwatchPlugin VERSION 0.1.1)

add_compile_options(-w) # Suppresses all warnings.

# Adding a directory with the library/application name as a subfolder of the
# include folder is a good practice. It helps avoid name clashes later on.
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/BlackwatchStudios")

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/info.plist" plist_content_to_merge)

# Adds a plugin target (that's basically what the Projucer does).
juce_add_plugin(${PROJECT_NAME}
    COMPANY_NAME "BlackwatchStudios" # change this
    BUNDLE_ID "com.blackwatch.juce"
    IS_SYNTH TRUE # may change this
    NEEDS_MIDI_INPUT TRUE # may change this
    PLUGIN_MANUFACTURER_CODE BLKW # change this
    PLUGIN_CODE BWST # change this
    FORMATS Standalone VST3 AU # may change this
    PRODUCT_NAME "BlackwatchPlugin" # change this
    NEEDS_WEBVIEW2 true
    PLIST_TO_MERGE "${plist_content_to_merge}"
    AU_MAIN_TYPE "kAudioUnitType_MusicDevice"
    COPY_PLUGIN_AFTER_BUILD TRUE
)


add_custom_command(TARGET "${PROJECT_NAME}" PRE_BUILD
        COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/pre-build.sh)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)


FetchContent_Declare(
  httplib
  GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
  GIT_TAG v0.12.3
)
FetchContent_MakeAvailable(httplib)

# Manually list all .h and .cpp files for the plugin
# If you are like me, you'll use globs for your sanity.
# Just ensure you employ CONFIGURE_DEPENDS so the build system picks up changes
# If you want to appease the CMake gods and avoid globs, manually add files like so:
# set(SourceFiles Source/PluginEditor.h Source/PluginProcessor.h Source/PluginEditor.cpp Source/PluginProcessor.cpp)
# file(GLOB_RECURSE SourceFiles CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/source/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/source/*.h")
set(SourceFiles
        source/BwRMS.h
        source/BwRMS.cpp
        source/BwLogger.h
        source/BwLogger.cpp
        source/MySynth.h
        source/MySynth.cpp
        source/PubSub.h
        source/PubSub.cpp
        source/IGuiModel.h
        source/IGuiModel.cpp
        source/AppSettings.h
        source/AppSettings.cpp
        source/Repo.h
        source/Repo.cpp
        source/Browser.h
        source/RmsChangedEvent.h
        source/RmsChangedEvent.cpp
        source/MainAudio.h
        source/MainAudio.cpp
        source/MainGui.h
        source/MainGui.cpp
)
target_sources(${PROJECT_NAME} INTERFACE ${SourceFiles})

juce_add_binary_data(MyBinaryData
        SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../gui-web/dist/index.html
        ${CMAKE_CURRENT_SOURCE_DIR}/../repo.json
)


# Sets the include directories of the plugin project.
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Links to all necessary dependencies. The present ones are recommended by JUCE.
# If you use one of the additional modules, like the DSP module, you need to specify it here.
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        juce::juce_core
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_dsp
        juce::juce_gui_basics
        juce::juce_gui_extra
        nlohmann_json::nlohmann_json
        httplib::httplib
        MyBinaryData
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# requirements for httplib
if(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()
if(OPENSSL_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)
endif()

# These definitions are recommended by JUCE.
target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        JUCE_WEB_BROWSER=1
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_USE_WIN_WEBVIEW2_WITH_STATIC_LINKING=1
        CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
        PRODUCT_NAME_WITHOUT_VERSION="Blackwatch"
)

# Enables all warnings and treats warnings as errors.
# This needs to be set up only for your projects, not 3rd party
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /Wall /WX)
    # Silences some deprecation warnings that (hopefully) will be fixed in a future release of JUCE.
    target_compile_definitions(${PROJECT_NAME}
        PRIVATE
            _SILENCE_CXX23_ALIGNED_STORAGE_DEPRECATION_WARNING)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# In Visual Studio this command provides a nice grouping of source files in "filters".
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/..)

#if(CMAKE_BUILD_TYPE STREQUAL "Release")
#    add_custom_command(TARGET "${PROJECT_NAME}_All" POST_BUILD
#            COMMAND ${CMAKE_COMMAND} -E echo "Running post-build script on Release build..."
#            COMMAND bash ${CMAKE_SOURCE_DIR}/source/build-installer.sh)
#endif()

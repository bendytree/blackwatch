# Version is needed by JUCE.
project(BlackwatchPlugin VERSION 0.1.1)

# If you are building a VST2 or AAX plugin, CMake needs to be told where to find these SDKs on your
# system. This setup should be done before calling `juce_add_plugin`.

# juce_set_vst2_sdk_path(...)
# juce_set_aax_sdk_path(...)

# Adding a directory with the library/application name as a subfolder of the
# include folder is a good practice. It helps avoid name clashes later on.
#set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/BlackwatchStudios")

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/info.plist" plist_content_to_merge)

# `juce_add_plugin` adds a static library target with the name passed as the first argument
# (${PROJECT_NAME} here). This target is a normal CMake target, but has a lot of extra properties set
# up by default. As well as this shared code static library, this function adds targets for each of
# the formats specified by the FORMATS arguments. This function accepts many optional arguments.
# Check the readme at `docs/CMake API.md` in the JUCE repo for the full list.

juce_add_plugin(${PROJECT_NAME}
    # VERSION ...                               # Set this if the plugin version is different to the project version
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    COMPANY_NAME "${PROJECT_NAME}"             # Specify the name of the plugin's author
    BUNDLE_ID "com.blackwatch.juce"
    NEEDS_WEBVIEW2 TRUE
    PLIST_TO_MERGE "${plist_content_to_merge}"
    AU_MAIN_TYPE "kAudioUnitType_MusicDevice"
    IS_SYNTH TRUE                               # Is this a synth or an effect?
    NEEDS_MIDI_INPUT TRUE                       # Does the plugin need midi input?
    # NEEDS_MIDI_OUTPUT TRUE/FALSE              # Does the plugin need midi output?
    # IS_MIDI_EFFECT TRUE/FALSE                 # Is this plugin a MIDI effect?
    # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
    COPY_PLUGIN_AFTER_BUILD TRUE                # Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE BLKW               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE Bwst                            # A unique four-character plugin id with exactly one upper-case character
                                                # GarageBand 10.3 requires the first letter to be upper-case, and the remaining letters to be lower-case
    FORMATS AU VST3 Standalone                  # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME "${PROJECT_NAME}")            # The name of the final executable, which can differ from the target name

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

# `juce_generate_juce_header` will create a JuceHeader.h for a given target, which will be generated
# into your build tree. This should be included with `#include <JuceHeader.h>`. The include path for
# this header will be automatically added to the target. The main function of the JuceHeader is to
# include all your JUCE module headers; if you're happy to include module headers directly, you
# probably don't need to call this.

# juce_generate_juce_header(${PROJECT_NAME})

# `target_sources` adds source files to a target. We pass the target that needs the sources as the
# first argument, then a visibility parameter for the sources which should normally be PRIVATE.
# Finally, we supply a list of source files that will be built into the target. This is a standard
# CMake command.

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
        source/SettingsRepo.h
        source/SettingsRepo.cpp
        source/AppSettings.h
        source/AppSettings.cpp
        source/Repo.h
        source/Repo.cpp
        source/Browser.h
        source/MidiChangedEvent.h
        source/MidiChangedEvent.cpp
        source/RmsChangedEvent.h
        source/RmsChangedEvent.cpp
        source/MyMidiProcessor.h
        source/MyMidiProcessor.cpp
        source/MainAudio.h
        source/MainAudio.cpp
        source/MainGui.h
        source/MainGui.cpp
)
target_sources(${PROJECT_NAME} PRIVATE ${SourceFiles})

# Sets the include directories of the plugin project.
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# `target_compile_definitions` adds some preprocessor definitions to our target. In a Projucer
# project, these might be passed in the 'Preprocessor Definitions' field. JUCE modules also make use
# of compile definitions to switch certain features on/off, so if there's a particular feature you
# need that's not on by default, check the module header for the correct flag to set here. These
# definitions will be visible both to your code, and also the JUCE module code, so for new
# definitions, pick unique names that are unlikely to collide! This is a standard CMake command.

target_compile_definitions(${PROJECT_NAME}
    PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=1  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_USE_WIN_WEBVIEW2_WITH_STATIC_LINKING=1
        CMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}"
        PRODUCT_NAME_WITHOUT_VERSION="Blackwatch"
    )

# If your target needs extra binary assets, you can add them here. The first argument is the name of
# a new static library target that will include all the binary resources. There is an optional
# `NAMESPACE` argument that can specify the namespace of the generated binary data class. Finally,
# the SOURCES argument should be followed by a list of source files that should be built into the
# static library. These source files can be of any kind (wav data, images, fonts, icons etc.).
# Conversion to binary-data will happen when your target is built.

juce_add_binary_data(MyBinaryData
    SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/../gui-web/dist/index.html
        ${CMAKE_CURRENT_SOURCE_DIR}/../repo.json
)

# `target_link_libraries` links libraries and JUCE modules to other libraries or executables. Here,
# we're linking our executable target to the `juce::juce_audio_utils` module. Inter-module
# dependencies are resolved automatically, so `juce_core`, `juce_events` and so on will also be
# linked automatically. If we'd generated a binary data target above, we would need to link to it
# here too. This is a standard CMake command.

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        MyBinaryData              # If we'd created a binary data target, we'd link to it here
        juce::juce_core
        juce::juce_audio_utils
        juce::juce_audio_processors
        juce::juce_dsp
        juce::juce_gui_basics
        juce::juce_gui_extra
        nlohmann_json::nlohmann_json
        httplib::httplib
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

        # requirements for httplib
if(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
endif()

if(OPENSSL_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::SSL OpenSSL::Crypto)
endif()

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
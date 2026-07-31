cmake_minimum_required(VERSION 3.15)
project(EaglesNightEQ VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

# You need JUCE installed or as a submodule. Adjust path as needed.
# Option 1: If JUCE is a git submodule at ${CMAKE_CURRENT_SOURCE_DIR}/JUCE
add_subdirectory(JUCE)

juce_add_plugin(EaglesNightEQ
    COMPANY_NAME "Custom"
    PLUGIN_MANUFACTURER_CODE Cstm
    PLUGIN_CODE Enq1
    FORMATS VST3 AU Standalone
    PRODUCT_NAME "Eagles Night EQ"
)

target_sources(EaglesNightEQ
    PRIVATE
        Source/PluginProcessor.cpp
        Source/PluginEditor.cpp
)

target_compile_definitions(EaglesNightEQ
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(EaglesNightEQ
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

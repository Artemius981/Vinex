cmake_minimum_required(VERSION 3.21)
project(Vinex VERSION 0.0.1)

find_package(JUCE CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 14)

juce_add_plugin(Vinex
        COMPANY_NAME "AutFly Audio"
        IS_SYNTH TRUE
        NEEDS_MIDI_INPUT FALSE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE
        COPY_PLUGIN_AFTER_BUILD FALSE
        PLUGIN_MANUFACTURER_CODE AFly
        PLUGIN_CODE Vinx
        FORMATS VST3 Standalone
        PRODUCT_NAME "Vinex")

juce_generate_juce_header(Vinex)

target_sources(Vinex
        PRIVATE
        src/gui/PluginEditor.cpp
        src/PluginProcessor.cpp)

target_compile_definitions(Vinex
        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(Vinex
        PRIVATE
        juce::juce_audio_utils
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

cmake_minimum_required(VERSION 3.21)
project(Vinex VERSION 0.0.1)

find_package(JUCE CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 14)

juce_add_plugin(Vinex
        COMPANY_NAME "hvid audio"
        IS_SYNTH TRUE
        NEEDS_MIDI_INPUT TRUE
        NEEDS_MIDI_OUTPUT FALSE
        IS_MIDI_EFFECT FALSE
        # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_MANUFACTURER_CODE Hvid
        PLUGIN_CODE VinX
        FORMATS VST3 Standalone
        PRODUCT_NAME "Vinex")

juce_generate_juce_header(Vinex)

juce_add_binary_data(VinexBinaryData
        SOURCES
        res/HankenGroteskRegular.ttf
        res/HankenGroteskSemiBold.ttf)

target_sources(Vinex
        PRIVATE
        src/PluginProcessor.cpp
        src/gui/PluginEditor.cpp

        src/gui/VinexColours.h
        src/gui/VinexLookAndFeel.cpp
        src/gui/Knob.cpp
        src/gui/Section.cpp
        src/gui/OscSection.cpp
        src/gui/HeaderPanel.cpp
        src/gui/PresetBrowser.cpp

        src/dsp/SynthVoice.cpp
        src/dsp/Wavetable.cpp
        src/dsp/WavetableOscillator.cpp
        src/dsp/Oversampling.cpp
        src/kissfft/kiss_fft.c
        src/kissfft/kiss_fftr.c
        src/Constants.h
        src/DspConstants.h

        src/service/PresetManager.cpp
        src/service/WavetableManager.cpp)

target_compile_definitions(Vinex
        PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(Vinex
        PRIVATE
        VinexBinaryData
        juce::juce_dsp
        juce::juce_audio_utils
        PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)

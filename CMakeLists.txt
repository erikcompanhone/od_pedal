cmake_minimum_required(VERSION 3.22)
project(ODPedal VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(JUCE_VST2_VERSIONS_DEPRECATED)

add_subdirectory(third_party/JUCE)

juce_add_plugin(ODPedal
    COMPANY_NAME "EriksPedals"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD TRUE
    PLUGIN_MANUFACTURER_CODE Ycmp
    PLUGIN_CODE Odpl
    FORMATS VST3
    PRODUCT_NAME "OD Pedal"
)

add_subdirectory(src)

target_link_libraries(ODPedal PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
)

target_compile_definitions(ODPedal PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
)

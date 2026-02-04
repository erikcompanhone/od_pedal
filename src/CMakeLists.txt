juce_add_binary_data(ODPedalBinaryData SOURCES
    resources/images/knob.png
    resources/images/bypass_up.png
    resources/images/pedal_body_on.png
    resources/images/pedal_body_off.png
)

target_link_libraries(ODPedal PRIVATE ODPedalBinaryData)

target_sources(ODPedal PRIVATE
    dsp/OverdriveDSP.h
    dsp/OverdriveDSP.cpp
    plugin/PluginProcessor.h
    plugin/PluginProcessor.cpp
    plugin/PluginEditor.h
    plugin/PluginEditor.cpp
    plugin/PluginParameters.h
    plugin/PluginParameters.cpp
    plugin/CustomLookAndFeel.h
    plugin/CustomLookAndFeel.cpp
)

/*
  ==============================================================================

    KOLFO.H
    Created: 26 Sep 2022
    Author:  Penofcilin, King of KO Audio
    Purpose: Provides a useful LFO class for use within a JUCE process block.

    Adapted from Viator DSP's LFOGenerator Module:
    https://github.com/landonviator

  ==============================================================================
*/

#pragma once

#ifndef KOLFO_h
#define KOLFO_h
#include <JuceHeader.h>

class KOLFO
{
public:
    void prepare(const juce::dsp::ProcessSpec& spec);

    float getNextValue();

    float getFrequency();
    void setFrequency(float newFrequency);

    enum class WaveType
    {
        Sine,
        Saw,
        NegSaw,
        Square
    };

    void setWaveType(WaveType newWaveType);

    void reset();

private:
    float period;
    float currentTime;
    float frequency;
    float sampleRate;

    std::function<float(float)> waveFunction;


};
#endif /* KOLFO_h */

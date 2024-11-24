/*
  ==============================================================================
    KOLFO.H
    Created: 26 Sep 2022
    Author:  Penofcilin, King of KO Audio
    Purpose: Provides a useful LFO class for use within a JUCE process block.
    Adapted from Viator DSP's LFOGenerator Module:
  ==============================================================================
*/


#pragma once
#ifndef KOLFO_h
#define KOLFO_h

#include <JuceHeader.h>

class KOLFO
{
public:

    
    std::function<float(float)> generator;
    
    void prepare(const juce::dsp::ProcessSpec& spec);

    void prepare(const float customSampleRate);

    //Resets the phase of the wave
    void reset();

    float getNextValue();

    //The available waveform types
    enum class WaveType
    {
        Sine,
        Cosine,
        Saw,
        SawDown,
        Square,
        //Random
    };

    void setFrequency(float newFrequency);
    float getFrequency();
    void setWaveType(WaveType newWaveType);

private:
    //Initialize sets the waveform of the LFO. setWaveType should be used publicly.
    void initialise(const std::function<float(float)>& function);

    float m_frequency;
    float sampleRate;
    float NormalizedFrequency;
    juce::Random rando;
    juce::dsp::Phase<float> phase;
};

#endif /* KOLFO*/

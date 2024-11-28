/*
  ==============================================================================
    KOLFO.H
    Created: 26 Sep 2022
    Author:  Penofcilin, King of KO Audio
    Purpose: Provides a useful LFO class for use within a JUCE process block.
    Adapted from Viator DSP's LFOGenerator Module:

    You can also use this as a regular oscillator if you want.
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
    
    //Get the LFO ready with a spec object
    void prepare(const juce::dsp::ProcessSpec& spec);

    //Get the LFO ready with a custom sample rate
    void prepare(const float customSampleRate);

    //Resets the phase of the wave
    void reset();

    //Returns the next value the oscillator will 'spit out'
    float getNextValue();

    //The available waveform types. Note if you're like me and didn't take trig until you're 22 these might be confusing,
    /*
        sine begins it's phase at 0 on an amplitude of -1 to 1, so on a scale of 0 to 1 thats actually 0.5 if you think about it.
        Cosine on the other hand either starts at -1 or 1, depending on negative or positive.
        Positive cosine starts off at 1, so thats maximum modulation.
        Negative cosine starts at -1, so on a scale of 0 to 1, that's no modulation.
    */
    enum WaveType
    {
        Sine = 0,
        Cosine = 1,
        NegativeCosine = 2,
        Saw = 3,
        SawDown = 4,
        Square = 5,
        InverseSquare = 6
        //Random is scary because it will pop and crack like crazy if done wrong
    };

    //Set the frequency of the oscillator in hz.
    void setFrequency(float newFrequency);
    //returns the current frequency of the oscillator
    float getFrequency();
    //Sets the wavetype for the oscillator.
    void setWaveType(WaveType newWaveType);
    //Meant to jump the wave ahead to some point in it's phase. Note that phase is a value between 0 and 2 pi.
    void advancePhase(float increment);

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

/*
  ==============================================================================
    KOLFO.H
    Created: 26 Sep 2022
    Author:  Penofcilin, King of KO Audio
    Purpose: Provides a useful LFO class for use within a JUCE process block.
    Adapted from Viator DSP's LFOGenerator Module

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
    /*The available waveform types.Note if you're like me and didn't take trig until you're 22 these might be confusing,
       sine begins it's phase at 0 on an amplitude of -1 to 1, so on a scale of 0 to 1 thats actually 0.5 if you think about it.
       Cosine on the other hand either starts at -1 or 1, depending on negative or positive.
       Positive cosine starts off at 1, so thats maximum modulation.
       Negative cosine starts at -1, so on a scale of 0 to 1, that's no modulation.
       Squares are squares. 
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
        //Random
    };

    //Returns a string array of wave types. Useful for putting in an APVTS parameter.
    static juce::StringArray GET_WAVE_TYPES();

    //Sets samplerate and resets phase of lfo.
    void prepare(const double customSampleRate);

    //Resets the phase of the wave.
    void reset();

    //Returns the next value of the oscillator and advances the phase.
    float getNextValue();

    //Returns next value and advances phase, use this when you need to use something in the process block but the buffer sizes are not fixed.
    float getNextValue(float bufferSize);

    //Meant to jump the wave ahead to some point in it's phase. Note that phase is a value between 0 and 2 pi.
    void advancePhase(float increment);

    //Set the frequency of the oscillator in hz.
    void setFrequency(double newFrequency);

    //returns the current frequency of the oscillator.
    float getFrequency();

    //Sets the samplerate to something new without resetting the phase.
    void setSampleRate(double newSampleRate);

    //Returns the samplerate assigned to the lfo
    double getSampleRate();

    //Sets the wavetype for the oscillator.
    void setWaveType(WaveType newWaveType);

    //Returns the LFO's current wavetype.
    WaveType getWaveType();

private:
    //Initialize sets the waveform of the LFO. setWaveType should be used publicly.
    void initialise(const std::function<double(double)>& function);

    double frequency;
    double sampleRate;
    double relativeFrequency;
    float increment{ 0 };
    //juce::Random rand;
    juce::dsp::Phase<float> phase;
    std::function<double(double)> generator;
    WaveType currentWavetype;
};

#endif /* KOLFO*/

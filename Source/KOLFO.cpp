#include "KOLFO.h"

//Use this is you will be using the LFO in your buffer loop
void  KOLFO::prepare(const juce::dsp::ProcessSpec& spec)
{
    sampleRate = spec.sampleRate / spec.maximumBlockSize;
    reset();
}

//Use this for every other case, I.E. calling once per processblock
void  KOLFO::prepare(const float customSampleRate)
{
    sampleRate = customSampleRate;
    reset();
}

void  KOLFO::reset()
{
    phase.reset();
}

//Set Wave Function for LFO
void  KOLFO::initialise(const std::function<float(float)>& function)
{
        generator = function;
}

//Returns the next value the LFO will spit out, which will always be a value between -1 and 1. Basically gives you the current y position of the wave.
//Some helpful stuff: std::rect() will make it a value between 0 and 1, which is useful for many operations. Adding 1 and dividing by 2 will do the same thing
//For instance LFO range is -1 to 1, add 1 to that its 0 to 2, times 0.5 its 0 to 1
//The way you should use this in the buffer loop is to first loop through for as many samples as are in the buffer, fill a vector with the values it spits out, then use it in your actual buffer loop
//if you don't do that, it will give you weird values because the buffer loop runs for each channel, which messes with the continuity of the wave
float  KOLFO::getNextValue()
{
    auto increment = juce::MathConstants<float>::twoPi * NormalizedFrequency;
    return generator(phase.advance(increment) - juce::MathConstants<float>::pi);
}

//Sets the frequency of the LFO in hertz
//If you don't set samplerate correctly, this won't work right
void KOLFO::setFrequency(float newFrequency)
{
    m_frequency = newFrequency; NormalizedFrequency = newFrequency/ sampleRate;
}

float  KOLFO::getFrequency()
{
    return m_frequency;
}

void  KOLFO::setWaveType(WaveType newWaveType)
{
    switch (newWaveType)
    {
    case  KOLFO::WaveType::Sine:
    {
        initialise([](float x) {return std::sin(x); });
        break;
    }
    case KOLFO::WaveType::Cosine:
    {
        initialise([](float x) {return std::cos(x); });
        break;
    }
    case KOLFO::WaveType::NegativeCosine:
    {
        initialise([](float x) {return (std::cos(x) * -1.f); });
        break;
    }
    case  KOLFO::WaveType::Saw:
    {
        initialise([](float x) {return x / juce::MathConstants<float>::pi; });
        break;
    }
    case  KOLFO::WaveType::SawDown:
    {
        initialise([](float x) {return (x / juce::MathConstants<float>::pi) * (-1); });
        break;
    }
    case  KOLFO::WaveType::Square:
    {
        initialise([](float x) {return x < 0.0f ? -1.0f : 1.0f; });
        break;
    }
    case  KOLFO::WaveType::InverseSquare:
    {
        initialise([](float x) {return x < 0.0f ? 1.0f : -1.0f; });
        break;
    }
    /*case  KOLFO::WaveType::Random:
    {
        initialise([&](float x) {return (x * rando.nextFloat()); });
    }*/
    }
}

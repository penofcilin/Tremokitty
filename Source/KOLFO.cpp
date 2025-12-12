#include "KOLFO.h"

juce::StringArray KOLFO::GET_WAVE_TYPES()
{
    return juce::StringArray("Sine", "Cosine", "NegativeCosine", "Saw", "SawDown", "Square", "InverseSquare");
}

void KOLFO::prepare(const double customSampleRate)
{
    sampleRate = customSampleRate;
    reset();
}

void  KOLFO::reset()
{
    phase.reset();
}

void  KOLFO::initialise(const std::function<double(double)>& function)
{
        generator = function;
}

//Returns the next value the LFO will spit out, which will always be a value between -1 and 1. Basically gives you the current y position of the wave.
//Some helpful stuff: std::rect() will make it a value between 0 and 1, which is useful for many operations. Adding 1 and dividing by 2 will do the same thing
//The way you should use this in the buffer loop is to first loop through for as many samples as are in the buffer, fill a vector with the values it spits out, then use it in your actual buffer loop
float  KOLFO::getNextValue()
{
    increment = juce::MathConstants<float>::twoPi * relativeFrequency;
    return generator(phase.advance(increment) - juce::MathConstants<float>::pi);
}

float KOLFO::getNextValue(float bufferSize)
{
    relativeFrequency = frequency / (sampleRate / bufferSize);
    increment = juce::MathConstants<float>::twoPi * relativeFrequency;
    return generator(phase.advance(increment) - juce::MathConstants<float>::pi);
}

void KOLFO::advancePhase(float increment)
{
    phase.advance(increment);
}

void KOLFO::setFrequency(double newFrequency)
{
    frequency = newFrequency; 
    relativeFrequency = newFrequency / sampleRate;
}

float  KOLFO::getFrequency()
{
    return frequency;
}

void KOLFO::setSampleRate(double newSampleRate)
{
    sampleRate = newSampleRate;
}

double KOLFO::getSampleRate()
{
    return sampleRate;
}

//This function returns an anonymous wave function
void  KOLFO::setWaveType(WaveType newWaveType)
{
    switch (newWaveType)
    {
    case  KOLFO::WaveType::Sine:
    {
        initialise([](float x) {return std::sin(x); });
        currentWavetype = WaveType::Sine;
        break;
    }
    case KOLFO::WaveType::Cosine:
    {
        initialise([](float x) {return std::cos(x); });
        currentWavetype = WaveType::Cosine;
        break;
    }
    case KOLFO::WaveType::NegativeCosine:
    {
        initialise([](float x) {return (std::cos(x) * -1.0); });
        currentWavetype = WaveType::NegativeCosine;
        break;
    }
    case  KOLFO::WaveType::Saw:
    {
        initialise([](float x) {return x / juce::MathConstants<float>::pi; });
        currentWavetype = WaveType::Saw;
        break;
    }
    case  KOLFO::WaveType::SawDown:
    {
        initialise([](float x) {return (x / juce::MathConstants<float>::pi) * (-1.0); });
        currentWavetype = WaveType::SawDown;
        break;
    }
    case  KOLFO::WaveType::Square:
    {
        initialise([](float x) {return x < 0.0 ? -1.0 : 1.0; });
        currentWavetype = WaveType::Square;
        break;
    }
    case  KOLFO::WaveType::InverseSquare:
    {
        initialise([](float x) {return x < 0.0 ? 1.0 : -1.0; });
        currentWavetype = WaveType::InverseSquare;
        break;
    }
    /*case  KOLFO::WaveType::Random:
    {
        initialise([&](float x) {return (x * rando.nextFloat()); });
    }*/
    }
}

KOLFO::WaveType KOLFO::getWaveType()
{
    return currentWavetype;
}

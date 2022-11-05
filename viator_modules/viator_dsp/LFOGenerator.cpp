#include "LFOGenerator.h"

void viator_dsp::LFOGenerator::prepare(const juce::dsp::ProcessSpec &spec)
{
    sampleRate = spec.sampleRate / spec.maximumBlockSize;
    reset();
}

void viator_dsp::LFOGenerator::prepare(const float customSampleRate)
{
    sampleRate = customSampleRate;

    reset();
}

void viator_dsp::LFOGenerator::reset()
{
    phase.reset();
}

//Set Wave Function for LFO
void viator_dsp::LFOGenerator::initialise (const std::function<float (float)>& function,
                 size_t lookupTableNumPoints)
{
    if (lookupTableNumPoints != 0)
    {
        auto* table = new juce::dsp::LookupTableTransform<float> (function, -juce::MathConstants<float>::pi, juce::MathConstants<float>::pi,
                                                             lookupTableNumPoints);

        lookupTable.reset (table);
        generator = [table] (float x) { return (*table) (x); };
    }
    else
    {
        //This one is the part that I use, simply sets the wavefunction to whatever function is passed as arg
        generator = function;
    }
}

float viator_dsp::LFOGenerator::processSample(float newInput)
{
    auto increment = juce::MathConstants<float>::twoPi * NormalizedFrequency;
    return newInput + generator (phase.advance (increment) - juce::MathConstants<float>::pi);
}

float viator_dsp::LFOGenerator::getNextValue()
{
    auto increment = juce::MathConstants<float>::twoPi * NormalizedFrequency;
    return generator(phase.advance(increment) - juce::MathConstants<float>::pi);
}

void viator_dsp::LFOGenerator::setParameter(ParameterId parameter, float parameterValue)
{
    switch (parameter)
    {
    case viator_dsp::LFOGenerator::ParameterId::kFrequency: m_frequency = parameterValue; NormalizedFrequency = parameterValue / sampleRate; break;
        case viator_dsp::LFOGenerator::ParameterId::kBypass: m_GlobalBypass = static_cast<bool>(parameterValue); break;
    }
}

float viator_dsp::LFOGenerator::getFrequency()
{
    return m_frequency;
}

void viator_dsp::LFOGenerator::setWaveType(WaveType newWaveType)
{
    switch (newWaveType)
    {
        case viator_dsp::LFOGenerator::WaveType::kSine:
        {
            //initialise([](float x){return std::sin(x); });
            //Fastsin is a little faster than std::sin
            initialise([this](float x) {return  fastSin(x); });
            break;
        }
        case viator_dsp::LFOGenerator::WaveType::kSaw:
        {
            initialise([](float x){return x / juce::MathConstants<float>::pi; });
            break;
        }
        case viator_dsp::LFOGenerator::WaveType::kSawDown:
        {
            initialise([](float x) {return (x / juce::MathConstants<float>::pi)* (-1); });
            break;
        }
        case viator_dsp::LFOGenerator::WaveType::kSquare:
        {
            initialise([](float x) {return x < 0.0f ? -1.0f : 1.0f; });
            break;
        }
        case viator_dsp::LFOGenerator::WaveType::kRandom:
        {
            initialise([&](float x) {return (x * rando.nextFloat()); });
        }
    }
}

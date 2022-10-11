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
        generator = function;
    }
    waveFunction = function;
}

float viator_dsp::LFOGenerator::processSample(float newInput)
{
    auto increment = juce::MathConstants<float>::twoPi * m_frequency / sampleRate;
    return newInput + generator (phase.advance (increment) - juce::MathConstants<float>::pi);
}

float viator_dsp::LFOGenerator::processSample(float newInput, float mod)
{
    auto increment = juce::MathConstants<float>::twoPi * m_frequency / sampleRate;
    return (newInput + generator(phase.advance(increment) - juce::MathConstants<float>::pi))*mod;
}

void viator_dsp::LFOGenerator::setParameter(ParameterId parameter, float parameterValue)
{
    switch (parameter)
    {
        case viator_dsp::LFOGenerator::ParameterId::kFrequency: m_frequency = parameterValue; break;
        case viator_dsp::LFOGenerator::ParameterId::kBypass: m_GlobalBypass = static_cast<bool>(parameterValue); break;
    }
}

float viator_dsp::LFOGenerator::getFrequency()
{
    return m_frequency;
}

float viator_dsp::LFOGenerator::getNextValue()
{
    inc = juce::MathConstants<float>::twoPi * m_frequency / sampleRate;
    myPhase += inc;
    if (myPhase > juce::MathConstants<float>::twoPi)
        myPhase -= juce::MathConstants<float>::twoPi;

    return waveFunction(myPhase);
}

void viator_dsp::LFOGenerator::setWaveType(WaveType newWaveType)
{
    switch (newWaveType)
    {
        case viator_dsp::LFOGenerator::WaveType::kSine:
        {
            initialise([](float x){return std::sin(x); });
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
        case viator_dsp::LFOGenerator::WaveType::kSmoothSquare:
        {
            //initialise([](float x){return x < 0.0f ? -1.0f : 1.0f; });
            //This function is much more expensive, but is a lot smoother, so it avoids clicking.
            initialise([](float x) {return tanh(50 * cos(x)); });
            break;
        }
    }
}

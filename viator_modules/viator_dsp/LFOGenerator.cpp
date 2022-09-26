#include "LFOGenerator.h"

void viator_dsp::LFOGenerator::prepare(const juce::dsp::ProcessSpec &spec)
{
    sampleRate = spec.sampleRate;
    period = 1.f / sampleRate;
    
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

float  viator_dsp::LFOGenerator::getNextValue()
{
    currentPhase += period;
    if (currentPhase >= 1.f)
        currentPhase = 0.f;
    float value = waveFunction(juce::MathConstants<float>::twoPi * m_frequency * currentPhase + 0.f);
    currentPhase += period;
    return value;
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

        case viator_dsp::LFOGenerator::WaveType::kNegSaw:
        {
            initialise([](float x) {return (x / juce::MathConstants<float>::pi)* (-1); });
            break;
        }
            
        case viator_dsp::LFOGenerator::WaveType::kSquare:
        {
            initialise([](float x){return x < 0.0f ? -1.0f : 1.0f; });
            break;
        }
    }
}

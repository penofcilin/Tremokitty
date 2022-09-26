#include "KOLFO.h"

//Prepares the samplerate, period, and current time variables.
void KOLFO::prepare(const juce::dsp::ProcessSpec& spec)
{
	sampleRate = spec.sampleRate;
	period = 1.f / sampleRate;
	currentTime = 0.f;
}

//Sets the current time (phase) to 0.
void KOLFO::reset()
{
	currentTime = 0;
}


//Applies the waveFunction to get the next value out of the LFO.
//Formula is 
float KOLFO::getNextValue()
{
	if (currentTime >= 1.f)
		currentTime = 0.f;
	float value = std::sin(juce::MathConstants<float>::twoPi * frequency * currentTime + 0.f);
	currentTime += period;
	return value;
}

//Returns Frequency
float KOLFO::getFrequency()
{
	return frequency;
}

//Sets frequency
void KOLFO::setFrequency(float newFrequency)
{
	frequency = newFrequency;
}

//Sets the wavetype of the LFO to the given type.
void KOLFO::setWaveType(WaveType newWaveType)
{
	switch (newWaveType)
	{
	case(WaveType::Sine):
		waveFunction = [](float x) {return std::sin(x); };
		break;
	case(WaveType::Saw):
		waveFunction = [](float x) {return x / juce::MathConstants<float>::pi; };
		break;
	case(WaveType::NegSaw):
		waveFunction = [](float x) {return (x / juce::MathConstants<float>::pi) * -1; };
		break;
	case(WaveType::Square):
		waveFunction = [](float x) {return x < 0.0f ? -1.0f : 1.0f; };
		break;
	}
}

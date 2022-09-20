/*
  ==============================================================================

    NoteDurations.cpp
    Created: 19 Sep 2022 8:20:00pm
    Author:  Levi

  ==============================================================================
*/

#include "NoteDurations.h"

NoteDurations::NoteDurations()
{
    DBG("Happy to initialize you suh!");
    NoteTypes = juce::StringArray{ "Half", "DHalf", "Quarter", "DQuarter", "TQuarter", "8th", "D8th", "T8th", "16th", "D16th", "T16th" };
}

juce::StringArray NoteDurations::getNoteTypes()
{
    return NoteTypes;
}

void NoteDurations::prepare(float bpm)
{
    BPM = bpm;
}

float NoteDurations::getHalfNote()
{
    return 120 / BPM;
}

float NoteDurations::getDottedHalfNote()
{
    return 120 / BPM + 60 / BPM;
}

float NoteDurations::getQuarterNote()
{
    return 60 / BPM;
}

float NoteDurations::getDottedQuarterNote()
{
    return 90 / BPM;
}

float NoteDurations::getTripletQuarterNote()
{
    return 40 / BPM;
}

float NoteDurations::getEighthNote()
{
    return 30 / BPM;
}

float NoteDurations::getDottedEighthNote()
{
    return 45 / BPM;
}

float NoteDurations::getTripletEighthNote()
{
    return 20 / BPM;
}

float NoteDurations::getSixteenthNote()
{
    return 15 / BPM;
}

float NoteDurations::getDottedSixteenthNote()
{
    return 22.5 / BPM;
}

float NoteDurations::getTripletSixteenthNote()
{
    return 10 / BPM;
}
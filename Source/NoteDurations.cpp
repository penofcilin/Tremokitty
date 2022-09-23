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
}

juce::StringArray NoteDurations::getNoteTypes()
{
    return juce::StringArray{ "Whole", "Half", "Dotted Half", "Quarter", "Dotted Quarter", "Triplet Quarter", "Eighth", "Dotted Eighth", "Triplet Eighth", "Sixteenth", "Dotted Sixteenth", "Triplet Sixteenth" };
}

// ;)
juce::StringArray NoteDurations::getStraightNoteTypes()
{
    return juce::StringArray("Free", "Whole", "Half", "Quarter", "Eighth", "Sixteenth");
}

juce::StringArray NoteDurations::getModdedNoteTypes()
{
    return juce::StringArray("Straight", "Dotted Half", "Dotted Quarter", "Triplet Quarter", "Dotted Eighth", "Triplet Eighth", "Dotted Sixteenth", "Triplet Sixteenth");
}

void NoteDurations::prepare(float bpm)
{
    BPM = bpm;
}

float NoteDurations::getWholeNote()
{
    return BPM / 240;
}

float NoteDurations::getHalfNote()
{
    return BPM/ 120;
}

float NoteDurations::getDottedHalfNote()
{
    return  (BPM / 120) + (BPM / 240);
}

float NoteDurations::getQuarterNote()
{
    return BPM / 60;
}

float NoteDurations::getDottedQuarterNote()
{
    return BPM / 90;
}

float NoteDurations::getTripletQuarterNote()
{
    return BPM / 40;
}

float NoteDurations::getEighthNote()
{
    return BPM / 30;
}

float NoteDurations::getDottedEighthNote()
{
    return BPM / 45;
}

float NoteDurations::getTripletEighthNote()
{
    return BPM / 20;
}

float NoteDurations::getSixteenthNote()
{
    return BPM / 15;
}

float NoteDurations::getDottedSixteenthNote()
{
    return BPM / 22.5;
}

float NoteDurations::getTripletSixteenthNote()
{
    return BPM / 10;
}
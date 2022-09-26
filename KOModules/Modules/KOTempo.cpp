/*
  ==============================================================================

    KOTempo.cpp
    Created: 26 Sep 2022 8:20:00pm
    Author:  Levi

  ==============================================================================
*/

#include "KOTempo.h"

//Returns a string containing all the possible note types
juce::StringArray KOTempo::getNoteTypes()
{
    return juce::StringArray{ "Whole", "Half", "Dotted Half", "Quarter", "Dotted Quarter", "Triplet Quarter", "Eighth", "Dotted Eighth", "Triplet Eighth", "Sixteenth", "Dotted Sixteenth", "Triplet Sixteenth" };
}

// Returns a string returning each of the note types, without modifications. Free indicates that there is no connection to the beat, I.e for a freerunning LFO.
juce::StringArray KOTempo::getStraightNoteTypes()
{
    return juce::StringArray("Free", "Whole", "Half", "Quarter", "Eighth", "Sixteenth");
}

//Returns a string array with all the modded kinds of notes. Straight indicates the note is not modded.
juce::StringArray KOTempo::getModdedNoteTypes()
{
    return juce::StringArray("Straight", "Dotted Half", "Dotted Quarter", "Triplet Quarter", "Dotted Eighth", "Triplet Eighth", "Dotted Sixteenth", "Triplet Sixteenth");
}

//Sets the BPM for the module. If this isn't set, then the get methods won't work properly.
void KOTempo::prepare(float bpm)
{
    BPM = bpm;
}

//The rest of the methods return the HZ duration of the particular type of note given the current BPM. For instance, a quarter note at 60 BPM is 1 hz.
float KOTempo ::getWholeNote()
{
    return BPM / 240;
}

float KOTempo ::getHalfNote()
{
    return BPM / 120;
}

float KOTempo  ::getDottedHalfNote()
{
    return  (BPM / 120) + (BPM / 240);
}

float KOTempo  ::getQuarterNote()
{
    return BPM / 60;
}

float KOTempo  ::getDottedQuarterNote()
{
    return BPM / 90;
}

float KOTempo  ::getTripletQuarterNote()
{
    return BPM / 40;
}

float KOTempo  ::getEighthNote()
{
    return BPM / 30;
}

float KOTempo  ::getDottedEighthNote()
{
    return BPM / 45;
}

float KOTempo  ::getTripletEighthNote()
{
    return BPM / 20;
}

float KOTempo  ::getSixteenthNote()
{
    return BPM / 15;
}

float KOTempo  ::getDottedSixteenthNote()
{
    return BPM / 22.5;
}

float KOTempo  ::getTripletSixteenthNote()
{
    return BPM / 10;
}

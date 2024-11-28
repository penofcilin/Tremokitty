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

juce::StringArray KOTempo::getNoteTypesAlternative()
{
    return juce::StringArray{ "1", "1/2", ".1/2", "1/4", ".1/4", "T1/4", "1/8", ".1/8", "T1/8", "1/16", ".1/16", "T1/16",};
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
void KOTempo::setBPM(float bpm)
{
    BPM = bpm;
}

void KOTempo::setSampleRate(float samplerate)
{
    sampleRate = samplerate;
}

float KOTempo::getNoteLengthHertz(NoteTypes type)
{
    switch (type) 
    {
    case NoteTypes::Whole:
        return BPM / 240;
        break;
    case NoteTypes::Half:
        return BPM / 120;
        break;
    case NoteTypes::DottedHalf:
        return  (BPM / 120) + (BPM / 240);
        break;
    case NoteTypes::Quarter:
        return BPM / 60;
        break;
    case NoteTypes::DottedQuarter:
        return BPM / 90;
        break;
    case NoteTypes::TripletQuarter:
        return BPM / 40;
        break;
    case NoteTypes::Eighth:
        return BPM / 30;
        break;
    case NoteTypes::DottedEighth:
        return BPM / 45;
        break;
    case NoteTypes::TripletEighth:
        return BPM / 20;
        break;
    case NoteTypes::Sixteenth:
        return BPM / 15;
        break;
    case NoteTypes::DottedSixteenth:
        return BPM / 22.5;
        break;
    case NoteTypes::TripletSixteenth:
        return BPM / 10;
        break;
    default:
        DBG("not given valid NoteType in getNoteLengthHertz");
        break;
    }
}

//returns the length of a note in samples, ie a whole note at 60 bpm at a 44100 samplerate will be 44100 samples long.
double KOTempo::getNoteLengthSamples(NoteTypes type)
{
    // Convert bpm to beats per second
    double beatsPerSecond = BPM / 60.0;

    // Calculate the duration of a single beat in seconds
    double beatDuration = 1.0 / beatsPerSecond;

    // Calculate the duration of the given note in seconds
    double noteDuration = 0;
    switch (type) {
    case NoteTypes::Whole:
        noteDuration = beatDuration * 4.0;
        break;
    case NoteTypes::Half:
        noteDuration = beatDuration * 2.0;
        break;
    case NoteTypes::DottedHalf:
        noteDuration = beatDuration * 3.0;
        break;
    case NoteTypes::Quarter:
        noteDuration = beatDuration;
        break;
    case NoteTypes::DottedQuarter:
        noteDuration = beatDuration * 1.5;
        break;
    case NoteTypes::TripletQuarter:
        noteDuration = beatDuration * 2.0 / 3.0;
        break;
    case NoteTypes::Eighth:
        noteDuration = beatDuration / 2.0;
        break;
    case NoteTypes::DottedEighth:
        noteDuration = beatDuration * 3.0 / 4.0;
        break;
    case NoteTypes::TripletEighth:
        noteDuration = beatDuration / 3.0;
        break;
    case NoteTypes::Sixteenth:
        noteDuration = beatDuration / 4.0;
        break;
    case NoteTypes::DottedSixteenth:
        noteDuration = beatDuration * 3.0 / 8.0;
        break;
    case NoteTypes::TripletSixteenth:
        noteDuration = beatDuration / 6.0;
        break;
    default:
        DBG("not given valid NoteType in getNoteLengthHertz");
        break;
    }

    // Convert the duration from seconds to samples and return it
    return round(noteDuration * sampleRate);
}

//Returns the length of a note in milliseconds
float KOTempo::getNoteLengthMS(NoteTypes type)
{
    // Convert bpm to beats per second
    double beatsPerSecond = BPM / 60.0;

    // Calculate the duration of a single beat in seconds
    double beatDuration = 1.0 / beatsPerSecond;

    // Calculate the duration of the given note in seconds
    double noteDuration = 0;
    switch (type) {
    case NoteTypes::Whole:
        noteDuration = beatDuration * 4;
        break;
    case NoteTypes::Half:
        noteDuration = beatDuration * 2;
        break;
    case NoteTypes::DottedHalf:
        noteDuration = beatDuration * 3;
        break;
    case NoteTypes::Quarter:
        noteDuration = beatDuration;
        break;
    case NoteTypes::DottedQuarter:
        noteDuration = beatDuration * 1.5;
        break;
    case NoteTypes::TripletQuarter:
        noteDuration = beatDuration * 2.0 / 3.0;
        break;
    case NoteTypes::Eighth:
        noteDuration = beatDuration / 2;
        break;
    case NoteTypes::DottedEighth:
        noteDuration = beatDuration * 3.0 / 4.0;
        break;
    case NoteTypes::TripletEighth:
        noteDuration = beatDuration / 3;
        break;
    case NoteTypes::Sixteenth:
        noteDuration = beatDuration / 4;
        break;
    case NoteTypes::DottedSixteenth:
        noteDuration = beatDuration * 3.0 / 8.0;
        break;
    case NoteTypes::TripletSixteenth:
        noteDuration = beatDuration / 6;
        break;
    default:
        DBG("Not given valid notetype in getNoteLengthMS");
        break;
    }

    // Convert the duration from seconds to milliseconds and return it
    return round(noteDuration * 1000);
}

float KOTempo::getSampleRate()
{
    return sampleRate;
}

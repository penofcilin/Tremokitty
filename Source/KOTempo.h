/*
==============================================================================
KOTempo.h
Created: 26 Sep 2022 3:51:21am
Author:  Penofcilin The King Of KO
Purpose: Provides useful tempo related methods, like getting the hertz values of notes. Make sure to set BPM and samplerate before using. 

hint you can easily find bpm in process block with following code:

(in processor header file)
KOTempo tempo;
juce::AudioPlayHead* playHead;
juce::AudioPlayHead::CurrentPositionInfo currentPosition;

(In processblock)
playHead = this->getPlayHead();
playHead->getCurrentPosition(currentPos);
tempo.setBPM(currentPos.bpm);
==============================================================================
*/

#pragma once
#ifndef KOTempo_h
#define KOTempo_h

#include <JuceHeader.h>

class KOTempo
{
public:
    enum NoteTypes {Whole = 0, Half = 1, DottedHalf = 2, Quarter = 3, DottedQuarter = 4, TripletQuarter = 5, Eighth = 6, DottedEighth = 7, TripletEighth = 8, Sixteenth = 9, DottedSixteenth = 10, TripletSixteenth = 11};

    //Get a string array of the available notetypes.
    static juce::StringArray getNoteTypes();

    //Get a string array of the available notetypes in short form, ie dotted quarter note = .1/4
    static juce::StringArray getNoteTypesAlternative();

    //Get a string array of the notetypes that are not dotted or triplets.
    static juce::StringArray getStraightNoteTypes();

    //Get a string array of the notetypes that are either dotted or triplets
    static juce::StringArray getModdedNoteTypes();

    //Return the length of a given note in hz
    float getNoteLengthHertz(NoteTypes type);

    //Return the length of a given note in samples
    double getNoteLengthSamples(NoteTypes type);

    //Return the length of a given note in milliseconds
    float getNoteLengthMS(NoteTypes type);

    //Set the samplerate of the current project
    void setSampleRate(float samplerate);

    //Returns the samplerate assigned to the tempo object. -1 if no samplerate was assigned.
    float getSampleRate();

    //Set the bpm of the current project
    void setBPM(float bpm);

    //Return the BPM associated with this tempo object
    float getBPM();

private:
    float BPM{ -1.f };
    float sampleRate{ -1.f };
};
#endif // !KOTempo_h

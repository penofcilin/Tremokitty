/*
==============================================================================

KOTempo.h
Created: 26 Sep 2022 3:51:21am
Author:  Penofcilin The King Of KO
Purpose: Provides useful tempo related methods.

==============================================================================
*/

#pragma once
#ifndef KOTempo_h
#define KOTempo_h

#include <JuceHeader.h>
class KOTempo
{
public:
    void prepare(float bpm);
    juce::StringArray getNoteTypes();
    juce::StringArray getStraightNoteTypes();
    juce::StringArray getModdedNoteTypes();

    float getWholeNote();
    float getHalfNote();
    float getDottedHalfNote();
    float getQuarterNote();
    float getDottedQuarterNote();
    float getTripletQuarterNote();
    float getEighthNote();
    float getDottedEighthNote();
    float getTripletEighthNote();
    float getSixteenthNote();
    float getDottedSixteenthNote();
    float getTripletSixteenthNote();

private:
    float BPM;
};
#endif // !KOTempo_h

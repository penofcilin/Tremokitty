/*
  ==============================================================================

    NoteDurations.h
    Created: 19 Sep 2022 8:20:00pm
    Author:  Levi

  ==============================================================================
*/

#pragma once

class NoteDurations
{
public:

    NoteDurations();

    void prepare(float bpm);

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


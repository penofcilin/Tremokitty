/*
  ==============================================================================

    MyLNF.h
    Created: 11 Oct 2022 1:50:13am
    Author:  lmend

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
namespace juce
{
    namespace Gui
    {
        class MyLNF : public juce::LookAndFeel_V4
        {
        public:
            MyLNF()
            {

            }
            void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
                                  float sliderPosProportional, float rotaryStartAngle,
                                  float rotaryEndAngle, juce::Slider&)
            {

            }
            void drawLinearSlider(juce::Graphics& g, int x, int y, int width, int height,
                                  float sliderPos,
                                  float minSliderPos,
                                  float maxSliderPos,
                                  const juce::Slider::SliderStyle style, juce::Slider& slider)
            {
                if (slider.isBar())
                {
                    g.setColour(slider.findColour(Slider::trackColourId));
                    g.fillRect(slider.isHorizontal() ? Rectangle<float>(static_cast<float> (x), (float)y + 0.5f, sliderPos - (float)x, (float)height - 1.0f)
                               : Rectangle<float>((float)x + 0.5f, sliderPos, (float)width - 1.0f, (float)y + ((float)height - sliderPos)));
                }
                else
                {
                    auto isTwoVal = (style == Slider::SliderStyle::TwoValueVertical || style == Slider::SliderStyle::TwoValueHorizontal);
                    auto isThreeVal = (style == Slider::SliderStyle::ThreeValueVertical || style == Slider::SliderStyle::ThreeValueHorizontal);

                    auto trackWidth = jmin(6.0f, slider.isHorizontal() ? (float)height * 0.25f : (float)width * 0.25f);

                    Point<float> startPoint(slider.isHorizontal() ? (float)x : (float)x + (float)width * 0.5f,
                                            slider.isHorizontal() ? (float)y + (float)height * 0.5f : (float)(height + y));

                    Point<float> endPoint(slider.isHorizontal() ? (float)(width + x) : startPoint.x,
                                          slider.isHorizontal() ? startPoint.y : (float)y);

                    Path backgroundTrack;
                    backgroundTrack.startNewSubPath(startPoint);
                    backgroundTrack.lineTo(endPoint);
                    g.setColour(slider.findColour(Slider::backgroundColourId));
                    g.strokePath(backgroundTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });

                    Path valueTrack;
                    Point<float> minPoint, maxPoint, thumbPoint;

                    if (isTwoVal || isThreeVal)
                    {
                        minPoint = { slider.isHorizontal() ? minSliderPos : (float)width * 0.5f,
                                     slider.isHorizontal() ? (float)height * 0.5f : minSliderPos };

                        if (isThreeVal)
                            thumbPoint = { slider.isHorizontal() ? sliderPos : (float)width * 0.5f,
                                           slider.isHorizontal() ? (float)height * 0.5f : sliderPos };

                        maxPoint = { slider.isHorizontal() ? maxSliderPos : (float)width * 0.5f,
                                     slider.isHorizontal() ? (float)height * 0.5f : maxSliderPos };
                    }
                    else
                    {
                        auto kx = slider.isHorizontal() ? sliderPos : ((float)x + (float)width * 0.5f);
                        auto ky = slider.isHorizontal() ? ((float)y + (float)height * 0.5f) : sliderPos;

                        minPoint = startPoint;
                        maxPoint = { kx, ky };
                    }

                    auto thumbWidth = getSliderThumbRadius(slider);

                    valueTrack.startNewSubPath(minPoint);
                    valueTrack.lineTo(isThreeVal ? thumbPoint : maxPoint);
                    g.setColour(slider.findColour(Slider::trackColourId));
                    g.strokePath(valueTrack, { trackWidth, PathStrokeType::curved, PathStrokeType::rounded });

                    if (!isTwoVal)
                    {
                        g.setColour(slider.findColour(Slider::thumbColourId));
                        g.fillEllipse(Rectangle<float>(static_cast<float> (thumbWidth), static_cast<float> (thumbWidth)).withCentre(isThreeVal ? thumbPoint : maxPoint));
                    }

                    if (isTwoVal || isThreeVal)
                    {
                        auto sr = jmin(trackWidth, (slider.isHorizontal() ? (float)height : (float)width) * 0.4f);
                        auto pointerColour = slider.findColour(Slider::thumbColourId);

                        if (slider.isHorizontal())
                        {
                            drawPointer(g, minSliderPos - sr,
                                        jmax(0.0f, (float)y + (float)height * 0.5f - trackWidth * 2.0f),
                                        trackWidth * 2.0f, pointerColour, 2);

                            drawPointer(g, maxSliderPos - trackWidth,
                                        jmin((float)(y + height) - trackWidth * 2.0f, (float)y + (float)height * 0.5f),
                                        trackWidth * 2.0f, pointerColour, 4);
                        }
                        else
                        {
                            drawPointer(g, jmax(0.0f, (float)x + (float)width * 0.5f - trackWidth * 2.0f),
                                        minSliderPos - trackWidth,
                                        trackWidth * 2.0f, pointerColour, 1);

                            drawPointer(g, jmin((float)(x + width) - trackWidth * 2.0f, (float)x + (float)width * 0.5f), maxSliderPos - sr,
                                        trackWidth * 2.0f, pointerColour, 3);
                        }
                    }
                }
            }


            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyLNF);
        };
    };
}
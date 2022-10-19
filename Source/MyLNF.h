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
            enum skins{sDefault, sHalloween, sChristmas, sSpace };
            skins currentSkin;
            juce::Image currentBgImage, defaultBg, christmasBg, halloweenBg, spaceBg;
            juce::String typeFace;
            juce::Colour textColour, backGroundColour, skinDefaultColor, skinComplementColor;

            MyLNF(skins skinToUse = skins::sDefault)
            {
                defaultBg = juce::ImageCache::getFromMemory(BinaryData::ChristmasBG_png, BinaryData::ChristmasBG_pngSize);
                christmasBg = juce::ImageCache::getFromMemory(BinaryData::ChristmasBG_png, BinaryData::ChristmasBG_pngSize);
                spaceBg = juce::ImageCache::getFromMemory(BinaryData::ChristmasBG_png, BinaryData::ChristmasBG_pngSize);
                halloweenBg = juce::ImageCache::getFromMemory(BinaryData::ChristmasBG_png, BinaryData::ChristmasBG_pngSize);
                /*halloweenBg = juce::ImageCache::getFromMemory(BinaryData::HalloweenBG_png, BinaryData::HalloweenBG_pngSize);
                spaceBg = juce::ImageCache::getFromMemory(BinaryData::SpaceBG_png, BinaryData::SpaceBG_pngSize);*/
                

                changeSkin(skinToUse);
            }

            void changeSkin(skins skinToUse)
            {
                switch (skinToUse)
                {
                case(skins::sHalloween):
                    setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::orange);
                    setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::black);
                    setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::orange);
                    currentBgImage = halloweenBg;
                    backGroundColour = juce::Colour::fromRGB(96, 8, 156);
                    skinDefaultColor = juce::Colours::black;
                    typeFace = "Algerian Regular";
                    textColour = juce::Colours::palevioletred;
                    setColour(juce::TextButton::ColourIds::textColourOnId, textColour);
                    setColour(juce::TextButton::ColourIds::textColourOffId, textColour);
                    setColour(juce::Label::ColourIds::textColourId, textColour);
                    setColour(juce::ToggleButton::ColourIds::textColourId, textColour);
                    break;
                case(skins::sChristmas):
                    setColour(juce::Slider::ColourIds::backgroundColourId,juce::Colours::goldenrod);
                    setColour(juce::Slider::ColourIds::trackColourId, juce::Colours::green);
                    setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::blue);
                    currentBgImage = christmasBg;
                    backGroundColour = juce::Colours::red;
                    skinDefaultColor = juce::Colours::green;
                    typeFace = "Avant Garde";
                    textColour = juce::Colours::lightgoldenrodyellow;
                    setColour(juce::TextButton::ColourIds::textColourOnId, textColour);
                    setColour(juce::TextButton::ColourIds::textColourOffId, textColour);
                    setColour(juce::Label::ColourIds::textColourId, textColour);
                    setColour(juce::ToggleButton::ColourIds::textColourId, textColour);
                    //setColour(juce::ComboBox::ColourIds::textColourId,)
                    break;
                case(skins::sSpace):
                    setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colours::white);
                    setColour(juce::Slider::ColourIds::trackColourId, juce::Colour::fromRGB(40, 29, 204));
                    setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour::fromRGB(40, 29, 204));
                    currentBgImage = spaceBg;
                    backGroundColour = juce::Colours::black;
                    skinDefaultColor = juce::Colour::fromRGB(40, 29, 204);
                    typeFace = "Avant Garde";
                    textColour = juce::Colours::white;
                    setColour(juce::TextButton::ColourIds::textColourOnId, textColour);
                    setColour(juce::TextButton::ColourIds::textColourOffId, textColour);
                    setColour(juce::Label::ColourIds::textColourId, textColour);
                   // setColour(juce::Label::ColourIds::)
                    setColour(juce::ToggleButton::ColourIds::textColourId, textColour);
                    break;
                default:
                    setColour(juce::Slider::ColourIds::backgroundColourId, juce::Colour::fromRGB(250, 225, 226));
                    setColour(juce::Slider::ColourIds::trackColourId, juce::Colour::fromRGB(132, 98, 103));
                    setColour(juce::Slider::ColourIds::thumbColourId, juce::Colour::fromRGB(132, 98, 103));
                    currentBgImage = defaultBg;
                    backGroundColour = juce::Colour::fromRGB(209, 179, 165);
                    skinDefaultColor = juce::Colour::fromRGB(156, 93, 112);
                    typeFace = "Calibri";
                    textColour = juce::Colours::black;
                    setColour(juce::TextButton::ColourIds::textColourOnId, textColour);
                    setColour(juce::TextButton::ColourIds::textColourOffId, textColour);
                    setColour(juce::Label::ColourIds::textColourId, textColour);
                    setColour(juce::ToggleButton::ColourIds::textColourId, textColour);
                }
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

            void drawComboBox(Graphics& g, int width, int height, bool,
                                              int, int, int, int, ComboBox& box)
            {
                auto cornerSize = box.findParentComponentOfClass<ChoicePropertyComponent>() != nullptr ? 0.0f : 3.0f;
                Rectangle<int> boxBounds(0, 0, width, height);

                g.setColour(skinDefaultColor);
                g.fillRoundedRectangle(boxBounds.toFloat(), cornerSize);

                g.setColour(skinDefaultColor);
                g.drawRoundedRectangle(boxBounds.toFloat().reduced(0.5f, 0.5f), cornerSize, 1.0f);

                Rectangle<int> arrowZone(width - 30, 0, 20, height);
                Path path;
                path.startNewSubPath((float)arrowZone.getX() + 3.0f, (float)arrowZone.getCentreY() - 2.0f);
                path.lineTo((float)arrowZone.getCentreX(), (float)arrowZone.getCentreY() + 3.0f);
                path.lineTo((float)arrowZone.getRight() - 3.0f, (float)arrowZone.getCentreY() - 2.0f);

                g.setColour(skinDefaultColor.withAlpha((box.isEnabled() ? 0.9f : 0.2f)));
                g.strokePath(path, PathStrokeType(2.0f));
            }
            void drawButtonBackground(Graphics& g,
                                                      Button& button,
                                                      const Colour& backgroundColour,
                                                      bool shouldDrawButtonAsHighlighted,
                                                      bool shouldDrawButtonAsDown)
            {
                auto cornerSize = 6.0f;
                auto bounds = button.getLocalBounds().toFloat().reduced(0.5f, 0.5f);

                auto baseColour = skinDefaultColor.withMultipliedSaturation(button.hasKeyboardFocus(true) ? 1.3f : 0.9f)
                    .withMultipliedAlpha(button.isEnabled() ? 1.0f : 0.5f);

                if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
                    baseColour = baseColour.contrasting(shouldDrawButtonAsDown ? 0.2f : 0.05f);

                g.setColour(baseColour);

                auto flatOnLeft = button.isConnectedOnLeft();
                auto flatOnRight = button.isConnectedOnRight();
                auto flatOnTop = button.isConnectedOnTop();
                auto flatOnBottom = button.isConnectedOnBottom();

                if (flatOnLeft || flatOnRight || flatOnTop || flatOnBottom)
                {
                    Path path;
                    path.addRoundedRectangle(bounds.getX(), bounds.getY(),
                                             bounds.getWidth(), bounds.getHeight(),
                                             cornerSize, cornerSize,
                                             !(flatOnLeft || flatOnTop),
                                             !(flatOnRight || flatOnTop),
                                             !(flatOnLeft || flatOnBottom),
                                             !(flatOnRight || flatOnBottom));

                    g.fillPath(path);

                    g.setColour(juce::Colours::black);
                    g.strokePath(path, PathStrokeType(1.0f));
                }
                else
                {
                    g.fillRoundedRectangle(bounds, cornerSize);

                    g.setColour(skinDefaultColor);
                    g.drawRoundedRectangle(bounds, cornerSize, 1.0f);
                }
            }




            JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MyLNF);
        };
    };
}
// src/components/Slider.jsx
import { useState } from "react";
import { emitSliderEvent } from "../../utilities/juceBridge.js";
import * as Slider from "@radix-ui/react-slider";
import "./tSlider.css";

export default function TSlider({
  id, // ParameterID.X
  min = 0,
  max = 1,
  step = 0.01,
  defaultValue = 0.5,
}) {
  const [value, setValue] = useState([defaultValue]);

  const handleChange = (newValue) => {
    setValue(newValue);
    emitSliderEvent(id, newValue[0]); // send scalar to JUCE
  };

  return (
    <Slider.Root
      className="tSlider"
      value={value}
      min={min}
      max={max}
      step={step}
      onValueChange={handleChange}
    >
      <Slider.Track className="tSliderTrack">
        <Slider.Range className="tSliderRange" />
      </Slider.Track>
      <Slider.Thumb className="tSliderThumb" />
    </Slider.Root>
  );
}

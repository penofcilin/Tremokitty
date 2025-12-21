// src/components/Slider.jsx
import { useState } from "react";
import { emitSliderEvent } from "../utilities/juceBridge.js";
import { Slider } from "@radix-ui/themes";
import "../styles/slider.css";

export default function TSlider({
  id, // ParameterID.X
  min = 0,
  max = 1,
  step = 0.01,
  defaultValue = 0.5,
  size = 3,
  variant = "soft",
}) {
  const [value, setValue] = useState([defaultValue]);

  const handleChange = (newValue) => {
    setValue(newValue);
    emitSliderEvent(id, newValue[0]); // send scalar to JUCE
  };

  return (
    <Slider
      value={value}
      min={min}
      max={max}
      step={step}
      size={size}
      variant={variant}
      onValueChange={handleChange}
      className="tSlider"
    />
  );
}

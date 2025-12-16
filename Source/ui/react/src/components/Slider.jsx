// src/components/Slider.jsx
import { useState } from "react";
import { emitSliderEvent } from "../utilities/juceBridge.js";
import "../styles/slider.css";

export default function Slider({
  id, // ParameterID.X
  label, // "Tremolo Rate"
  min = 0,
  max = 1,
  step = 0.01,
  defaultValue = 0.5,
  radius = 8, // corner radius
}) {
  const [value, setValue] = useState(defaultValue);

  const handleChange = (e) => {
    const newValue = parseFloat(e.target.value);
    setValue(newValue);
    emitSliderEvent(id, newValue); // Send to JUCE backend
  };

  return (
    <div className="sliderContainer">
      <label className="sliderLabel">{label}</label>

      <input
        type="range"
        min={min}
        max={max}
        step={step}
        value={value}
        onChange={handleChange}
        className="prettySlider"
        style={{ borderRadius: radius }}
      />

      <div className="sliderValue">{value.toFixed(2)}</div>
    </div>
  );
}

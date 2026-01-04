// src/components/Slider.jsx
import { useState } from "react";
import * as Slider from "@radix-ui/react-slider";
import * as Tooltip from "@radix-ui/react-tooltip";
import TTooltip from "../Tooltip/TTooltip.jsx";
import { emitSliderEvent } from "../../utilities/juceBridge.js";
import "./tSlider.css";

export default function TSlider({
  id, // ParameterID.X
  min = 0,
  max = 1,
  step = 0.1,
  defaultValue = 0.5,
  orientation = "horizontal",
  disabled = false,
  onChange,
  tooltip,
  tooltipMap,
  style,
}) {
  const [value, setValue] = useState([defaultValue]);
  const [isHovering, setIsHovering] = useState(false);
  const [isDragging, setIsDragging] = useState(false);

  const handleChange = (newValue) => {
    const v = newValue[0];

    setValue([v]);
    emitSliderEvent(id, v); //Emit a normalized value

    onChange?.(v);
  };

  const tooltipEnabled = Boolean(tooltip);
  const tooltipContent = tooltipMap
    ? tooltipMap(value[0])
    : value[0].toFixed(2);

  return (
    <Tooltip.Provider delayDuration={tooltip?.delay ?? 15}>
      <Tooltip.Root open={isHovering || isDragging}>
        <Tooltip.Trigger asChild>
          <Slider.Root
            className={`tSlider ${orientation} ${disabled ? "isDisabled" : ""}`}
            style={style}
            orientation={orientation}
            value={value}
            min={min}
            max={max}
            step={step}
            onValueChange={handleChange}
            onPointerEnter={() => setIsHovering(true)}
            onPointerLeave={() => setIsHovering(false)}
            onPointerDown={() => setIsDragging(true)}
            onPointerUp={() => setIsDragging(false)}
            onDoubleClick={() => {
              setValue([defaultValue]);
              handleChange([defaultValue]);
            }}
          >
            <Slider.Track className="tSliderTrack">
              <Slider.Range className="tSliderRange" />
            </Slider.Track>

            {/* Anchor = thumb */}
            <Slider.Thumb className="tSliderThumb" />
          </Slider.Root>
        </Tooltip.Trigger>

        {tooltipEnabled && (
          <Tooltip.Portal>
            <Tooltip.Content
              className="TooltipContent"
              side={tooltip?.side ?? "top"}
              sideOffset={8}
            >
              {tooltipContent}
              <Tooltip.Arrow className="TooltipArrow" />
            </Tooltip.Content>
          </Tooltip.Portal>
        )}
      </Tooltip.Root>
    </Tooltip.Provider>
  );
}

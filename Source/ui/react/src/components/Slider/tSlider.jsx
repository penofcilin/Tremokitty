// src/components/Slider.jsx
import { useState } from "react";
import * as Slider from "@radix-ui/react-slider";
import * as Tooltip from "@radix-ui/react-tooltip";
import TTooltip from "../Tooltip/TTooltip.jsx";
import { emitSliderEvent } from "../../utilities/juceBridge.js";
import "./tSlider.css";
import { normToSkewed } from "../../Utilities/General.js";

export default function TSlider({
  id, // ParameterID.X
  step = 0.0001,
  defaultValue = 0.5,
  value: controlledValue, // ✅ controlled support
  skew = 0,
  orientation = "horizontal",
  disabled = false,
  onChange,
  tooltip,
  tooltipMap,
  style,
}) {
  const [internalValue, setInternalValue] = useState([defaultValue]);
  const [isHovering, setIsHovering] = useState(false);
  const [isDragging, setIsDragging] = useState(false);

  const min = 0,
    max = 1;

  const value =
    controlledValue !== undefined ? [controlledValue] : internalValue;

  const handleChange = (newValue) => {
    const v = newValue[0];

    if (controlledValue === undefined) {
      setInternalValue([v]);
    }

    emitSliderEvent(id, v); // Emit normalized value
    onChange?.(v);
  };

  const tooltipEnabled = Boolean(tooltip);

  const displayValue = skew ? normToSkewed(value[0], 0, 1, skew) : value[0];

  const tooltipContent = tooltipMap
    ? tooltipMap(displayValue)
    : displayValue.toFixed(2);

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
              const v = defaultValue;

              if (controlledValue === undefined) {
                setInternalValue([v]);
              }

              emitSliderEvent(id, v);
              onChange?.(v);
            }}
          >
            <Slider.Track className="tSliderTrack">
              <Slider.Range className="tSliderRange" />
            </Slider.Track>

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

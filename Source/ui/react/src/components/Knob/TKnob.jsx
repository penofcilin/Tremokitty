// src/components/Knob/TKnob.jsx
import { useRef, useState } from "react";
import * as Tooltip from "@radix-ui/react-tooltip";
import { emitSliderEvent } from "../../utilities/juceBridge.js";
import { normToSkewed } from "../../Utilities/General.js";
import "./tKnob.css";

export default function TKnob({
  id,
  step = 0.001,
  defaultValue = 0.5,
  skew = 0,
  value: controlledValue,
  tooltip,
  tooltipMap,
  style,
  onChange,
}) {
  const min = 0,
    max = 1;
  const [internalValue, setInternalValue] = useState(defaultValue);
  const value = controlledValue !== undefined ? controlledValue : internalValue;

  const [isHovering, setIsHovering] = useState(false);
  const [isDragging, setIsDragging] = useState(false);

  const startY = useRef(0);
  const startValue = useRef(value);

  const clamp = (v) => Math.min(max, Math.max(min, v));

  const onPointerDown = (e) => {
    startY.current = e.clientY;
    startValue.current = value;
    setIsDragging(true);
    e.currentTarget.setPointerCapture(e.pointerId);
  };

  const onPointerUp = (e) => {
    e.currentTarget.releasePointerCapture(e.pointerId);
    setIsDragging(false);
  };

  const onPointerMove = (e) => {
    if (!e.buttons) return;

    const delta = startY.current - e.clientY;
    const range = max - min;
    const next = startValue.current + (delta / 150) * range;
    const snapped = Math.round(next / step) * step;
    const clamped = clamp(snapped);

    if (controlledValue === undefined) {
      setInternalValue(clamped);
    }

    emitSliderEvent(id, clamped);
    onChange?.(clamped);
  };

  const angle = -135 + ((value - min) / (max - min)) * 270;

  const tooltipEnabled = Boolean(tooltip);

  const tooltipContent =
    typeof tooltipMap === "function"
      ? tooltipMap(skew > 0 ? normToSkewed(value, min, max, skew) : value)
      : (skew > 0 ? normToSkewed(value, min, max, skew) : value).toFixed(2);

  return (
    <Tooltip.Provider delayDuration={tooltip?.delay ?? 15}>
      <Tooltip.Root open={tooltipEnabled && (isHovering || isDragging)}>
        <Tooltip.Trigger asChild>
          <div
            className="tKnob"
            onPointerEnter={() => setIsHovering(true)}
            onPointerLeave={() => setIsHovering(false)}
            onPointerDown={onPointerDown}
            onPointerUp={onPointerUp}
            onPointerMove={onPointerMove}
            onDoubleClick={() => {
              const v = clamp(defaultValue);

              if (controlledValue === undefined) {
                setInternalValue(v);
              }

              emitSliderEvent(id, v);
              onChange?.(v);
            }}
            style={{
              "--angle": `${angle}deg`,
              ...style,
            }}
          >
            <div className="tKnobDial">
              <div className="tKnobIndicator" />
            </div>
          </div>
        </Tooltip.Trigger>

        {tooltipEnabled && (
          <Tooltip.Portal className="tooltipBox">
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

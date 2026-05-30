import { useState } from "react";
import * as Tooltip from "@radix-ui/react-tooltip";
import { emitButtonEvent } from "../../utilities/juceBridge.js";
import { Button } from "@radix-ui/themes";
import "./tButton.css";

export default function TButton({
  id,
  size = 3,
  variant = "solid",
  emitEvent = true,
  radius = "medium",
  value,
  onChange,
  clickEvent,
  style,
  icon = null,
  isToggle = 0,
  tooltip,
  children,
}) {
  const [internalToggled, setInternalToggled] = useState(false);
  const [isHovering, setIsHovering] = useState(false);

  const isControlled = value !== undefined;
  const toggled = isControlled ? value : internalToggled;
  const tooltipEnabled = Boolean(tooltip);

  const button = (
    <Button
      style={style}
      className={`tButton ${isToggle ? (toggled ? "Toggled" : "") : ""}`}
      size={size}
      variant={variant}
      radius={radius}
      onPointerEnter={() => setIsHovering(true)}
      onPointerLeave={() => setIsHovering(false)}
      onClick={() => {
        if (emitEvent) {
          emitButtonEvent(id, isToggle);
        }

        if (isToggle) {
          const next = !toggled;

          if (!isControlled) {
            setInternalToggled(next);
          }

          onChange?.(next);
        }

        clickEvent?.();
      }}
    >
      {icon && <span className="tButtonIcon">{icon}</span>}
      {children && <span className="tButtonText">{children}</span>}
    </Button>
  );

  if (!tooltipEnabled) {
    return button;
  }

  return (
    <Tooltip.Provider delayDuration={tooltip?.delay ?? 15}>
      <Tooltip.Root open={isHovering}>
        <Tooltip.Trigger asChild>{button}</Tooltip.Trigger>

        <Tooltip.Portal>
          <Tooltip.Content
            className="TooltipContent"
            side={tooltip?.side ?? "top"}
            sideOffset={8}
          >
            {tooltip?.content ?? tooltip}
            <Tooltip.Arrow className="TooltipArrow" />
          </Tooltip.Content>
        </Tooltip.Portal>
      </Tooltip.Root>
    </Tooltip.Provider>
  );
}

import { useState } from "react";
import { emitButtonEvent } from "../../utilities/juceBridge.js";
import { Button } from "@radix-ui/themes";
import "./tButton.css";

export default function TButton({
  id,
  size = 3,
  variant = "solid",
  emitEvent = true,
  radius = "medium",

  // NEW: controlled toggle support
  value, // boolean | undefined
  onChange, // (nextBool) => void

  clickEvent,
  style,
  icon = null,
  isToggle = 0,
  children,
}) {
  const [internalToggled, setInternalToggled] = useState(false);

  const isControlled = value !== undefined;
  const toggled = isControlled ? value : internalToggled;

  return (
    <Button
      style={style}
      className={`tButton ${isToggle ? (toggled ? "Toggled" : "") : ""}`}
      size={size}
      variant={variant}
      radius={radius}
      onClick={() => {
        if (emitEvent) {
          emitButtonEvent(id, isToggle);
        }

        if (isToggle) {
          const next = !toggled;

          // update internal only if uncontrolled
          if (!isControlled) {
            setInternalToggled(next);
          }

          // notify parent in controlled mode (and also useful in uncontrolled)
          onChange?.(next);
        }

        clickEvent?.();
      }}
    >
      {icon && <span className="tButtonIcon">{icon}</span>}
      {children && <span className="tButtonText">{children}</span>}
    </Button>
  );
}

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
  clickEvent,
  style,
  icon = null,
  isToggle = 0,
  children,
}) {
  const [toggled, setToggled] = useState(false);
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
          setToggled(!toggled);
        }
        clickEvent?.();
      }}
    >
      {icon && <span className="tButtonIcon">{icon}</span>}
      {children && <span className="tButtonText">{children}</span>}
    </Button>
  );
}

import { emitButtonEvent } from "../../utilities/juceBridge.js";
import { Button } from "@radix-ui/themes";
import "./tButton.css";

export default function TButton({
  id,
  size = 3,
  variant = "solid",
  emitEvent = true,
  radius = "medium",
  style,
  icon = null,
  children,
}) {
  return (
    <Button
      style={style}
      className="tButton"
      size={size}
      variant={variant}
      radius={radius}
      onClick={() => {
        if (emitEvent) {
          emitButtonEvent(id, 0);
        }
      }}
    >
      {icon && <span className="tButtonIcon">{icon}</span>}
      {children && <span className="tButtonText">{children}</span>}
    </Button>
  );
}

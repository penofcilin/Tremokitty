import { emitButtonEvent } from "../../utilities/juceBridge.js";
import { Button } from "@radix-ui/themes";
import "./tButton.css";

export default function TButton({
  text,
  id,
  size = 3,
  variant = "solid",
  emitEvent = true,
  radius = "medium",
}) {
  return (
    <div>
      <Button
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
        {text}
      </Button>
    </div>
  );
}

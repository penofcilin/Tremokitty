import { emitButtonEvent } from "../utilities/juceBridge.js";
import { Button } from "@radix-ui/themes";

export default function TButton({
  text,
  id,
  size = 3,
  variant = "solid",
  radius = "medium",
}) {
  return (
    <div>
      <Button
        size={size}
        variant={variant}
        radius={radius}
        onClick={() => {
          emitButtonEvent(id, 0);
        }}
      >
        {text}
      </Button>
    </div>
  );
}

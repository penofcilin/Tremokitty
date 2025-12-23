import { emitButtonEvent } from "../../utilities/juceBridge.js";
import { Checkbox, Text } from "@radix-ui/themes";
import "./TCheckbox.css";

export default function TCheckbox({
  label,
  id,
  size = 2,
  variant = "surface",
  weight = "medium",
}) {
  return (
    <div>
      <Text className="tLabel" as="label" weight={weight} size={size}>
        <Checkbox
          className="checkBox"
          size={size}
          variant={variant}
          onClick={() => {
            emitButtonEvent(id, 1);
          }}
        ></Checkbox>
        {label}
      </Text>
    </div>
  );
}

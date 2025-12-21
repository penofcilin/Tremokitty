import { emitButtonEvent } from "../utilities/juceBridge.js";
import { Checkbox, Flex, Text } from "@radix-ui/themes";

export default function TCheckBox({
  label,
  id,
  size = 2,
  variant = "surface",
  weight = "medium",
}) {
  return (
    <div>
      <Text as="label" weight={weight} size={size}>
        <Checkbox
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

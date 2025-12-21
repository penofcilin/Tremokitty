import { useState } from "react";
import { emitDropdownEvent } from "../utilities/juceBridge.js";
import { DropdownMenu, Button, Heading } from "@radix-ui/themes";

export default function TDropdown({
  id,
  options = [],
  defaultValue = options[0],
  variant = "solid",
  size = 2,
}) {
  const [value, setValue] = useState(defaultValue);

  return (
    <div>
      <DropdownMenu.Root>
        <DropdownMenu.Trigger>
          <Button variant="soft">
            {value}
            <DropdownMenu.TriggerIcon />
          </Button>
        </DropdownMenu.Trigger>

        <DropdownMenu.Content size={size} variant={variant}>
          {options.map((choice, index) => (
            <DropdownMenu.Item
              key={choice}
              onSelect={() => {
                setValue(choice);
                emitDropdownEvent(id, index);
              }}
            >
              {choice}
            </DropdownMenu.Item>
          ))}
        </DropdownMenu.Content>
      </DropdownMenu.Root>
    </div>
  );
}

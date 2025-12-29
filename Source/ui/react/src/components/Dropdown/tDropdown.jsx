import { useState } from "react";
import { emitDropdownEvent } from "../../utilities/juceBridge.js";
import { DropdownMenu, Button } from "@radix-ui/themes";
import "./TDropdown.css";

export default function TDropdown({
  id,
  options = [],
  defaultValue = options[0],
  variant = "solid",
  size = 2,
  buttonStyle,
}) {
  const [value, setValue] = useState(defaultValue);

  return (
    <div>
      <DropdownMenu.Root className="dropdownRoot">
        <DropdownMenu.Trigger style={buttonStyle}>
          <Button className="triggerButton" variant="soft">
            {value}
            <DropdownMenu.TriggerIcon />
          </Button>
        </DropdownMenu.Trigger>

        <DropdownMenu.Content size={size} variant={variant}>
          {options.map((choice, index) => (
            <DropdownMenu.Item
              className="dropdownItem"
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

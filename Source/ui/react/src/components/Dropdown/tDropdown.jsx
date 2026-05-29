import { useEffect, useState } from "react";
import { emitDropdownEvent } from "../../utilities/juceBridge.js";
import { DropdownMenu, Button } from "@radix-ui/themes";
import "./TDropdown.css";

export default function TDropdown({
  id,
  options = [],
  defaultValue = options[0],
  variant = "solid",
  disabled = false,
  size = 2,
  buttonStyle,
  valueFromParent,
}) {
  const [value, setValue] = useState(defaultValue);

  useEffect(() => {
    setValue(valueFromParent);
  }, [setValue, valueFromParent]);

  return (
    <div>
      <DropdownMenu.Root className="dropdownRoot">
        <DropdownMenu.Trigger style={buttonStyle}>
          <Button
            className={`triggerButton ${disabled ? "disabled" : ""}`}
            variant="soft"
          >
            {value}
            <DropdownMenu.TriggerIcon />
          </Button>
        </DropdownMenu.Trigger>

        <DropdownMenu.Content size={size} variant={variant}>
          {Array.isArray(options) && options.length > 0 ? (
            options.map((choice, index) => (
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
            ))
          ) : (
            <DropdownMenu.Item disabled>No options</DropdownMenu.Item>
          )}
        </DropdownMenu.Content>
      </DropdownMenu.Root>
    </div>
  );
}

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

  const getDisplayName = (choice) =>
    String(choice ?? "")
      .split("/")
      .pop();

  useEffect(() => {
    setValue(valueFromParent);
  }, [valueFromParent]);

  const groupedOptions = {
    root: [],
    folders: {},
  };

  if (Array.isArray(options)) {
    options.forEach((choice, index) => {
      const parts = String(choice).split("/");

      if (parts.length === 1) {
        groupedOptions.root.push({
          label: choice,
          value: choice,
          index,
        });
      } else {
        const folder = parts[0];
        const label = parts.slice(1).join("/");

        if (!groupedOptions.folders[folder]) {
          groupedOptions.folders[folder] = [];
        }

        groupedOptions.folders[folder].push({
          label,
          value: choice,
          index,
        });
      }
    });
  }

  const handleSelect = (choice, index) => {
    setValue(choice);
    emitDropdownEvent(id, index);
  };

  return (
    <div>
      <DropdownMenu.Root className="dropdownRoot">
        <DropdownMenu.Trigger style={buttonStyle}>
          <Button
            className={`triggerButton ${disabled ? "disabled" : ""}`}
            variant="soft"
            disabled={disabled}
          >
            {getDisplayName(value)}
            <DropdownMenu.TriggerIcon />
          </Button>
        </DropdownMenu.Trigger>

        <DropdownMenu.Content size={size} variant={variant}>
          {Array.isArray(options) && options.length > 0 ? (
            <>
              {Object.entries(groupedOptions.folders).map(
                ([folderName, folderOptions]) => (
                  <DropdownMenu.Sub key={folderName}>
                    <DropdownMenu.SubTrigger className="subFolder">
                      {folderName}
                    </DropdownMenu.SubTrigger>

                    <DropdownMenu.SubContent>
                      {folderOptions.map((choice) => (
                        <DropdownMenu.Item
                          className="dropdownItem"
                          key={choice.value}
                          onSelect={() =>
                            handleSelect(choice.value, choice.index)
                          }
                        >
                          {choice.label}
                        </DropdownMenu.Item>
                      ))}
                    </DropdownMenu.SubContent>
                  </DropdownMenu.Sub>
                ),
              )}

              {Object.keys(groupedOptions.folders).length > 0 &&
                groupedOptions.root.length > 0 && <DropdownMenu.Separator />}

              {groupedOptions.root.map((choice) => (
                <DropdownMenu.Item
                  className="dropdownItem"
                  key={choice.value}
                  onSelect={() => handleSelect(choice.value, choice.index)}
                >
                  {choice.label}
                </DropdownMenu.Item>
              ))}
            </>
          ) : (
            <DropdownMenu.Item disabled>No options</DropdownMenu.Item>
          )}
        </DropdownMenu.Content>
      </DropdownMenu.Root>
    </div>
  );
}

import * as ToggleGroup from "@radix-ui/react-toggle-group";
import { emitTogglegroupEvent } from "../../utilities/juceBridge.js";
import "./TToggleGroup.css";

export default function TToggleGroup({
  id,
  value, // MUST be a valid option value
  options = [],
  ariaLabel = "Toggle group",
  onChange,
}) {
  const handleChange = (newValue) => {
    if (newValue == null) return; // prevent deselection

    const index = options.findIndex((opt) => opt.value === newValue);

    if (index !== -1) {
      emitTogglegroupEvent(id, index);
    }

    onChange?.(newValue);
  };

  return (
    <ToggleGroup.Root
      className="ToggleGroup"
      type="single"
      value={value}
      onValueChange={handleChange}
      aria-label={ariaLabel}
    >
      {options.map((opt) => (
        <ToggleGroup.Item
          key={opt.value}
          className="ToggleGroupItem"
          value={opt.value}
          aria-label={opt.label}
          disabled={opt.disabled}
        >
          {opt.icon ?? opt.label}
        </ToggleGroup.Item>
      ))}
    </ToggleGroup.Root>
  );
}

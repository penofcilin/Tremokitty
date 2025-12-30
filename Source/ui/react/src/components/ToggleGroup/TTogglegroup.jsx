import * as ToggleGroup from "@radix-ui/react-toggle-group";
import { emitTogglegroupEvent } from "../../utilities/juceBridge.js";
import "./TToggleGroup.css";

export default function TToggleGroup({
  id,
  value,
  options = [],
  ariaLabel = "Toggle group",
  onChange,
  vertical = false,
}) {
  // 🔑 GUARANTEE a valid value at all times
  const safeValue = options.some((opt) => opt.value === value)
    ? value
    : options[0]?.value;

  const handleChange = (newValue) => {
    // 🔑 Radix sends null when clicking the active item
    if (newValue == null) return;

    const index = options.findIndex((opt) => opt.value === newValue);
    if (index === -1) return;

    emitTogglegroupEvent(id, index);
    onChange?.(newValue);
  };

  return (
    <ToggleGroup.Root
      className={`ToggleGroup ${vertical ? "vertical" : ""}`}
      type="single"
      value={safeValue} // 🔒 never invalid
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

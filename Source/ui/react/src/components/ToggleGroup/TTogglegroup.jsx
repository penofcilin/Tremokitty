import * as ToggleGroup from "@radix-ui/react-toggle-group";
import { emitTogglegroupEvent } from "../../utilities/juceBridge.js";
import TTooltip from "../Tooltip/TTooltip.jsx";
import "./TToggleGroup.css";

//NOTE: VALUE MUST BE PASSED AS A STRING
export default function TToggleGroup({
  id,
  value,
  options = [],
  ariaLabel = "Toggle group",
  onChange,
  vertical = false,
  showTooltips = true,
  style,
}) {
  const safeValue = options.some((opt) => opt.value === value)
    ? value
    : options[0]?.value;

  const handleChange = (newValue) => {
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
      value={safeValue}
      onValueChange={handleChange}
      aria-label={ariaLabel}
      style={style}
    >
      {options.map((opt) => (
        <ToggleGroup.Item
          key={opt.value}
          className="ToggleGroupItem"
          value={opt.value}
          aria-label={opt.label}
          disabled={opt.disabled}
        >
          <TTooltip
            content={showTooltips ? opt.label : ""}
            side={vertical ? "right" : "top"}
            delay={400}
          >
            <span
              style={{
                display: "flex",
                alignItems: "center",
                justifyContent: "center",
                width: "100%",
                height: "100%",
              }}
            >
              {opt.icon ?? opt.label}
            </span>
          </TTooltip>
        </ToggleGroup.Item>
      ))}
    </ToggleGroup.Root>
  );
}

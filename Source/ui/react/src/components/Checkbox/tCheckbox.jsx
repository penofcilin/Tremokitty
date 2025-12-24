import * as Checkbox from "@radix-ui/react-checkbox";
import { CheckIcon } from "@radix-ui/react-icons";
import { emitButtonEvent } from "../../utilities/juceBridge.js";
import "./TCheckbox.css";

export default function TCheckbox({
  label,
  id,
  icon,
  size = 2, // you can map this to CSS if you want
  weight = "medium",
}) {
  return (
    <div className="tCheckboxWrapper">
      <label className="tCheckboxLabel">
        <Checkbox.Root
          className="tCheckboxRoot"
          id={id}
          onCheckedChange={(checked) => {
            emitButtonEvent(id, checked ? 1 : 0);
          }}
        >
          <Checkbox.Indicator className="tCheckboxIndicator">
            <CheckIcon />
          </Checkbox.Indicator>
        </Checkbox.Root>

        <span className="tCheckboxText">{label}</span>
      </label>
    </div>
  );
}

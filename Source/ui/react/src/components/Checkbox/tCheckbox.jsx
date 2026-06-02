import * as Checkbox from "@radix-ui/react-checkbox";
import { CheckIcon } from "@radix-ui/react-icons";
import { emitButtonEvent } from "../../utilities/juceBridge.js";
import "./TCheckbox.css";

export default function TCheckbox({
  label,
  id,
  name,
  checked,
  defaultChecked,
  onCheckedChange,
  Icon = CheckIcon,
}) {
  return (
    <div className="tCheckboxWrapper">
      <label className="tCheckboxLabel">
        <Checkbox.Root
          className="tCheckboxRoot"
          id={id}
          name={name}
          checked={checked}
          defaultChecked={defaultChecked}
          onCheckedChange={(checkedValue) => {
            const isChecked = checkedValue === true;

            onCheckedChange?.(isChecked);

            emitButtonEvent(id, 1, isChecked);
          }}
        >
          <Checkbox.Indicator className="tCheckboxIndicator">
            <Icon style={{ color: "white" }} />
          </Checkbox.Indicator>
        </Checkbox.Root>
      </label>
    </div>
  );
}

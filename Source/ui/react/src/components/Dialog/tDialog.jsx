import { useState } from "react";
import * as Dialog from "@radix-ui/react-dialog";
import * as Tooltip from "@radix-ui/react-tooltip";
import { emitFormEvent } from "../../utilities/juceBridge";
import { Box, Button } from "@radix-ui/themes";
import { TCheckbox, TToggleGroup } from "../../components";
import { Cross2Icon } from "@radix-ui/react-icons";
import "./tDialog.css";

export default function TDialog({
  id,
  header,
  description,
  fields = [],
  buttonStyle,
  buttonVariant = "solid",
  submitButtonText = "Save",
  icon = null,
  children,
  triggerTooltip,
}) {
  const [open, setOpen] = useState(false);

  const [formValues, setFormValues] = useState(() =>
    Object.fromEntries(
      fields.map((field) => [
        field.name,
        field.defaultValue ?? field.options?.[0]?.value ?? "",
      ]),
    ),
  );

  const handleSubmit = (e) => {
    e.preventDefault();

    const form = e.currentTarget;
    const formData = new FormData(form);
    const values = {};

    fields.forEach((field) => {
      const element = form.elements[field.name];

      if (field.type === "checkbox") {
        values[field.name] = element.checked;
      } else if (field.type === "toggleGroup") {
        values[field.name] = formValues[field.name];
      } else if (field.type === "number") {
        values[field.name] = Number(formData.get(field.name));
      } else {
        values[field.name] = formData.get(field.name);
      }
    });

    emitFormEvent(id, values);
    setOpen(false);
  };

  const triggerButton = (
    <Button style={buttonStyle} variant={buttonVariant} className="Button">
      {icon && <span className="tDialogIcon">{icon}</span>}
      {children && <span className="tDialogText">{children}</span>}
    </Button>
  );

  const trigger = triggerTooltip ? (
    <Tooltip.Provider delayDuration={triggerTooltip?.delay ?? 15}>
      <Tooltip.Root>
        <Tooltip.Trigger asChild>
          <Dialog.Trigger asChild>{triggerButton}</Dialog.Trigger>
        </Tooltip.Trigger>

        <Tooltip.Portal>
          <Tooltip.Content
            className="TooltipContent"
            side={triggerTooltip?.side ?? "top"}
            sideOffset={8}
          >
            {triggerTooltip?.content ?? triggerTooltip}
            <Tooltip.Arrow className="TooltipArrow" />
          </Tooltip.Content>
        </Tooltip.Portal>
      </Tooltip.Root>
    </Tooltip.Provider>
  ) : (
    <Dialog.Trigger asChild>{triggerButton}</Dialog.Trigger>
  );

  return (
    <Dialog.Root open={open} onOpenChange={setOpen}>
      {trigger}

      <Dialog.Portal>
        <Dialog.Overlay className="DialogOverlay" />

        <Dialog.Content className="DialogContent">
          <Box className="HeaderContainer" style={{ marginBottom: "18px" }}>
            <Dialog.Title className="DialogTitle">{header}</Dialog.Title>
          </Box>

          {description && (
            <Dialog.Description className="DialogDescription">
              {description}
            </Dialog.Description>
          )}

          <form onSubmit={handleSubmit}>
            {fields.map((field) => (
              <fieldset className="Fieldset" key={field.name}>
                {field.type === "checkbox" ? (
                  <label className="CheckboxRow" htmlFor={field.name}>
                    <TCheckbox
                      id={field.name}
                      name={field.name}
                      defaultChecked={field.defaultValue ?? false}
                      size="3"
                      variant="solid"
                    />
                    <span>{field.label}</span>
                  </label>
                ) : (
                  <>
                    <label className="Label" htmlFor={field.name}>
                      {field.label}
                    </label>

                    {field.type === "togglegroup" ? (
                      <TToggleGroup
                        id={field.name}
                        value={formValues[field.name]}
                        options={field.options}
                        vertical={field.vertical ?? false}
                        onChange={(newValue) => {
                          setFormValues((prev) => ({
                            ...prev,
                            [field.name]: newValue,
                          }));
                        }}
                      />
                    ) : field.type === "select" ? (
                      <select
                        className="Input"
                        id={field.name}
                        name={field.name}
                        defaultValue={field.defaultValue}
                      >
                        {field.options?.map((option) => (
                          <option key={option} value={option}>
                            {option}
                          </option>
                        ))}
                      </select>
                    ) : (
                      <input
                        className="Input"
                        id={field.name}
                        name={field.name}
                        type={field.type ?? "text"}
                        placeholder={field.placeholder}
                        defaultValue={field.defaultValue}
                        required={field.required}
                      />
                    )}
                  </>
                )}
              </fieldset>
            ))}

            <div
              style={{
                display: "flex",
                marginTop: 25,
                justifyContent: "flex-end",
              }}
            >
              <button className="Button save" type="submit">
                {submitButtonText}
              </button>
            </div>
          </form>

          <Dialog.Close asChild>
            <button className="IconButton" aria-label="Close">
              <Cross2Icon />
            </button>
          </Dialog.Close>
        </Dialog.Content>
      </Dialog.Portal>
    </Dialog.Root>
  );
}

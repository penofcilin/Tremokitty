import { useState } from "react";
import * as Dialog from "@radix-ui/react-dialog";
import * as Tooltip from "@radix-ui/react-tooltip";
import { emitFormEvent } from "../../utilities/juceBridge";
import { Box, Button } from "@radix-ui/themes";
import { Cross2Icon } from "@radix-ui/react-icons";
import "./tDialog.css";

export default function TDialog({
  id,
  header,
  description,
  fields = [],
  buttonStyle,
  icon = null,
  children,
  triggerTooltip,
}) {
  const [open, setOpen] = useState(false);

  const handleSubmit = (e) => {
    e.preventDefault();

    const formData = new FormData(e.currentTarget);
    const values = Object.fromEntries(formData.entries());

    emitFormEvent(id, values);
    setOpen(false);
  };

  const triggerButton = (
    <Button style={buttonStyle} className="Button">
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
          <Box className="HeaderContainer">
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
                <label className="Label" htmlFor={field.name}>
                  {field.label}
                </label>

                <input
                  className="Input"
                  id={field.name}
                  name={field.name}
                  type={field.type ?? "text"}
                  defaultValue={field.defaultValue}
                />
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
                Save
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

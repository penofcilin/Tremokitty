import { useEffect, useState } from "react";
import * as Dialog from "@radix-ui/react-dialog";
import { emitTogglegroupEvent } from "../../utilities/juceBridge";
import { Box, Button } from "@radix-ui/themes";
import { TCheckbox, TToggleGroup } from "../../components";
import { Cross2Icon } from "@radix-ui/react-icons";
import "./tDialog.css";

export default function SettingsDialog({
  header = "Settings",
  description,
  buttonStyle,
  buttonVariant = "solid",
  submitButtonText = "Save",
  icon = null,
  children,
  currentState,
}) {
  const SETTINGS_FIELDS = {
    SHOWANIMATIONS: {
      name: "SHOWANIMATIONS",
      label: "Show Animations",
      defaultValue: true,
    },

    SELECTEDPALLETTE: {
      name: "SELECTEDPALLETTE",
      label: "Color Palette",
      defaultValue: 0,
    },
  };

  const PALETTE_OPTIONS = [
    { value: 0, label: "Default", color: "var(--accent-primary)" },
    { value: 1, label: "Blue", color: "#3b82f6" },
    { value: 2, label: "Green", color: "#22c55e" },
  ];
  const [open, setOpen] = useState(false);

  const [showAnimations, setShowAnimations] = useState(
    SETTINGS_FIELDS.SHOWANIMATIONS.defaultValue,
  );

  const [selectedPalette, setSelectedPalette] = useState(
    SETTINGS_FIELDS.SELECTEDPALLETTE.defaultValue,
  );

  useEffect(() => {
    if (!currentState?.GLOBALSETTINGS) return;

    setShowAnimations(currentState.GLOBALSETTINGS.SHOWANIMATIONS ?? true);
    setSelectedPalette(currentState.GLOBALSETTINGS.PALLETTECHOICE ?? 0);
  }, [currentState]);

  const handleSetSelectedPalette = (value) => {
    setSelectedPalette(value);
    emitTogglegroupEvent("SELECTEDPALLETTE", value);
  };

  const handleSubmit = (e) => {
    e.preventDefault();

    setOpen(false);
  };

  const triggerButton = (
    <Button style={buttonStyle} variant={buttonVariant} className="Button">
      {icon && <span className="tDialogIcon">{icon}</span>}
      {children && <span className="tDialogText">{children}</span>}
    </Button>
  );

  const trigger = <Dialog.Trigger asChild>{triggerButton}</Dialog.Trigger>;

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
            <fieldset className="Fieldset">
              <label
                className="CheckboxRow"
                htmlFor={SETTINGS_FIELDS.SHOWANIMATIONS.name}
              >
                <TCheckbox
                  id={SETTINGS_FIELDS.SHOWANIMATIONS.name}
                  name={SETTINGS_FIELDS.SHOWANIMATIONS.name}
                  checked={showAnimations}
                  onCheckedChange={setShowAnimations}
                  size="3"
                  variant="solid"
                />
                <span style={{ marginLeft: "7px", fontWeight: "bold" }}>
                  Show Animations
                </span>
              </label>
            </fieldset>

            <fieldset className="Fieldset">
              <label className="Label">Color Palette</label>

              <div className="PaletteGroup">
                {PALETTE_OPTIONS.map((palette) => (
                  <button
                    key={palette.value}
                    type="button"
                    className={`PaletteOption ${
                      selectedPalette === palette.value ? "selected" : ""
                    }`}
                    style={{
                      "--palette-color": palette.color,
                    }}
                    aria-label={palette.label}
                    onClick={() => handleSetSelectedPalette(palette.value)}
                  >
                    <span
                      className="PaletteSwatch"
                      style={{ backgroundColor: palette.color }}
                    />
                  </button>
                ))}
              </div>
            </fieldset>

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

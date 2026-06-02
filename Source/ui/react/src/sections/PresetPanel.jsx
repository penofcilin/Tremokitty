import { useEffect } from "react";
import { Flex } from "@radix-ui/themes";
import { TDialog, TDropdown, TButton } from "../components";
import { ElementID } from "../utilities/juceBridge";
import { FileIcon, CaretUpIcon, CaretDownIcon } from "@radix-ui/react-icons";
import { Folder } from "lucide-react";

export default function PresetPanel({
  style,
  currentState,
  providedPresets,
  selectedPresetIndex,
  setSelectedPresetIndex,
}) {
  const presets = Array.isArray(providedPresets?.[0])
    ? providedPresets[0]
    : (providedPresets ?? []);

  // Obtaining initial preset index
  useEffect(() => {
    if (!currentState) return;

    const index = currentState.INITPRESETINDEX;
    setSelectedPresetIndex(Number(index));
  }, [currentState, setSelectedPresetIndex]);

  const fieldFormat = [
    {
      name: "PresetName",
      label: "Name",
      type: "text",
      placeholder: "",
      defaultValue: "",
      description: "Shown publicly",
      required: false,
    },
    {
      name: "PresetCategory",
      label: "Category",
      type: "text",
      placeholder: "",
      defaultValue: "",
      description: "Shown publicly",
      required: false,
    },
  ];

  return (
    <Flex
      style={{
        display: "inline-flex",
        padding: "4px",
        border: "2px solid white",
        borderRadius: "10px",
        alignItems: "center",
        transform: "translateX(90px)",
      }}
      gap={0}
    >
      <Flex direction="row" align="center" gap={0}>
        <TDropdown
          id="presetDropdown"
          defaultValue="Default"
          options={presets}
          valueFromParent={presets[selectedPresetIndex] ?? "Default"}
          buttonStyle={{ height: "40px" }}
        />

        {/* Preset stepper */}
        <Flex
          direction="column"
          height="40px"
          justify="start"
          style={{ marginLeft: "2px" }}
        >
          <TButton
            style={{
              width: "20px",
              height: "20px",
              padding: 0,
            }}
            tooltip="Previous Preset"
            icon={<CaretUpIcon width={20} height={20} />}
            id={ElementID.PREVIOUSPRESETBUTTON}
          />

          <TButton
            style={{
              width: "20px",
              height: "20px",
              padding: 0,
            }}
            tooltip="Next Preset"
            icon={<CaretDownIcon width={20} height={20} />}
            id={ElementID.NEXTPRESETBUTTON}
          />
        </Flex>

        <TDialog
          id={ElementID.SAVEPRESETBUTTON}
          header="Save Preset"
          description=""
          fields={fieldFormat}
          triggerTooltip="Save Preset"
          buttonText="Save"
          buttonStyle={{
            height: "40px",
            width: "40px",
            marginLeft: "3px",
            backgroundColor: "var(--accent-primary)",
          }}
          icon={<FileIcon width={25} height={25} stroke={5} />}
        />

        <TButton
          id={ElementID.OPENPRESETFOLDERBUTTON}
          style={{
            height: "40px",
            width: "40px",
            marginLeft: "3px",
          }}
          tooltip="Open Preset Folder"
        >
          <Folder />
        </TButton>

        <TButton
          id={ElementID.RESETPRESETBUTTON}
          style={{
            height: "40px",
            width: "50px",
            marginLeft: "3px",
          }}
          tooltip="Reset to Default Preset"
        >
          Reset
        </TButton>
      </Flex>
    </Flex>
  );
}

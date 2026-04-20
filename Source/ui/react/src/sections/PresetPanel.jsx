import { useContext, useState, useEffect } from "react";
import { PresetsContext } from "../utilities/PresetsContext";
import { Flex } from "@radix-ui/themes";
import { TDialog, TDropdown, TButton } from "../components";
import { ElementID } from "../utilities/juceBridge";
import { FileIcon, CaretUpIcon, CaretDownIcon } from "@radix-ui/react-icons";

export default function PresetPanel({ style }) {
  const [selectedPresetIndex, setSelectedPresetIndex] = useState(1);

  const presetContext = useContext(PresetsContext);
  const presets = presetContext.presets[0];

  //updateUI event listener
  useEffect(() => {
    const handler = (data) => {
      setSelectedPresetIndex(data.PRESETINDEX);
    };

    window.__JUCE__.backend.addEventListener("UpdateUI", handler);

    return () => {
      window.__JUCE__.backend.removeEventListener("UpdateUI", handler);
    };
  }, []);

  const fieldFormat = [
    {
      name: "PresetName", // REQUIRED (form key)
      label: "Name", // REQUIRED (visible label)
      type: "text", // optional (default: "text")
      placeholder: "",
      defaultValue: "",
      description: "Shown publicly",
      required: false,
    },
    {
      name: "Category", // REQUIRED (form key)
      label: "Category", // REQUIRED (visible label)
      type: "text", // optional (default: "text")
      placeholder: "",
      defaultValue: "",
      description: "Shown publicly",
      required: false,
    },
  ];
  return (
    <Flex
      style={{
        display: "inline-flex", // keep it tight around contents
        padding: "4px", // space between border and controls
        border: "2px solid white", // the white box outline
        borderRadius: "10px",
        alignItems: "center",
        transform: "translateX(90px)",
      }}
      gap={0}
    >
      <Flex direction="row" align="center" gap={0}>
        <TDropdown
          id="presetDropdown"
          defaultValue={"Default"}
          options={presets}
          valueFromParent={presets[selectedPresetIndex]}
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
            icon={<CaretUpIcon width={20} height={20} />}
            id={ElementID.PREVIOUSPRESETBUTTON}
          />
          <TButton
            style={{
              width: "20px",
              height: "20px",
              padding: 0,
            }}
            icon={<CaretDownIcon width={20} height={20} />}
            id={ElementID.NEXTPRESETBUTTON}
          />
        </Flex>

        <TDialog
          id={ElementID.SAVEPRESETBUTTON}
          header="Save Preset"
          description=""
          fields={fieldFormat}
          buttonText="Save"
          buttonStyle={{ height: "40px", width: "40px", marginLeft: "3px" }}
          icon={<FileIcon width={25} height={25} stroke={5} />}
        ></TDialog>

        <TButton
          id={ElementID.RESETPRESETBUTTON}
          style={{ height: "40px", width: "50px", marginLeft: "3px" }}
          text="Reset"
        >
          Reset
        </TButton>
      </Flex>
    </Flex>
  );
}

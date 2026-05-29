import { useContext, useState, useEffect } from "react";
import { PresetsContext } from "../utilities/PresetsContext";
import { Flex } from "@radix-ui/themes";
import { TDialog, TDropdown, TButton } from "../components";
import { ElementID } from "../utilities/juceBridge";
import { FileIcon, CaretUpIcon, CaretDownIcon } from "@radix-ui/react-icons";

export default function PresetPanel({ style, currentState, providedPresets }) {
  const [selectedPresetIndex, setSelectedPresetIndex] = useState(1);
  const [presets, setPresets] = useState([]);

  //Obtaining initial preset index
  useEffect(() => {
    if (!currentState) return;

    const index = currentState.INITPRESETINDEX;

    setSelectedPresetIndex(Number(index));
  }, [currentState]);

  //Obtaining updated list of presets
  useEffect(() => {
    if (providedPresets) {
      console.log("Updating Presets:", providedPresets);
      if (Array.isArray(providedPresets[0])) {
        setPresets(providedPresets[0]);
      } else {
        setPresets(providedPresets);
      }
    }
  }, [providedPresets]);

  //Preset changing listener, for updating preset after saving
  useEffect(() => {
    const handler = (newPresetIndex) => {
      setSelectedPresetIndex(newPresetIndex);
    };

    window.__JUCE__.backend.addEventListener("PresetIndexUpdate", handler);

    return () => {
      window.__JUCE__.backend.removeEventListener("PresetIndexUpdate", handler);
    };
  }, []);

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

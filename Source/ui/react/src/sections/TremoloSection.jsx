import { useState } from "react";
import { ParameterID, WaveTypes } from "../utilities/juceBridge.js";
import { ReloadIcon, PersonIcon, SunIcon } from "@radix-ui/react-icons";
import { Box, Flex, Heading } from "@radix-ui/themes";
import {
  TSlider,
  TDropdown,
  TCheckbox,
  TDialog,
  TToggleGroup,
} from "../components";

export default function TremoloSection(data) {
  const presets = data.data.Presets[0];
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
  const [waveType, setWaveType] = useState(WaveTypes[0]);

  return (
    <div>
      <Flex direction="column" gap="3" width="250px">
        <Box
          p="2"
          display="inline-block"
          style={{ backgroundColor: "pink", borderRadius: "14px" }}
        >
          <Heading>Tremolo Section</Heading>
          <TDialog
            id="savePresetForm"
            header="Save Preset"
            description=""
            fields={fieldFormat}
            buttonText="Save Preset"
          ></TDialog>
          <Heading>Tremolo Rate</Heading>
          <TSlider
            id={ParameterID.TREMRATE}
            min={0}
            max={20}
            step={0.011}
            defaultValue={10}
            size="3"
            variant="soft"
            tooltip={{
              enabled: true,
            }}
          ></TSlider>

          <Heading>Presets</Heading>
          <TDropdown id="presetDropdown" options={presets}></TDropdown>

          <Heading>Wave Type</Heading>
          <TToggleGroup
            id={ParameterID.TREMWAVE}
            value={waveType}
            onChange={setWaveType}
            options={[
              {
                value: "Sine",
                label: "Sine",
                icon: <ReloadIcon></ReloadIcon>,
              },
              {
                value: "Saw",
                label: "Saw",
                icon: <PersonIcon></PersonIcon>,
              },
              {
                value: "Square",
                label: "Square",
                icon: <SunIcon></SunIcon>,
              },
            ]}
          ></TToggleGroup>

          <TCheckbox label="Bypass Tremolo" id={ParameterID.TREMBP}></TCheckbox>
        </Box>
      </Flex>
    </div>
  );
}

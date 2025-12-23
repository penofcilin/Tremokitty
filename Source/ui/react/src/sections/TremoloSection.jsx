import { ParameterID, ElementID } from "../utilities/juceBridge.js";
import { Box, Flex, Heading } from "@radix-ui/themes";
import { TSlider, TDropdown, TButton, TCheckbox, TDialog } from "../components";

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
  return (
    <div>
      <Flex direction="column" gap="3">
        <Box
          p="2"
          width="250px"
          height="235px"
          style={{ backgroundColor: "pink" }}
        >
          <Heading>Tremolo Section</Heading>
          <TDialog
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
            defaultValue={0.5}
            size="3"
            variant="soft"
          ></TSlider>

          <Heading>Presets</Heading>

          <TCheckbox label="Bypass Tremolo" id={ParameterID.TREMBP}></TCheckbox>
        </Box>
      </Flex>
    </div>
  );
}

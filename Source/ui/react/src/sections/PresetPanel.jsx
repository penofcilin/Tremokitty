import { Flex } from "@radix-ui/themes";
import { TDialog } from "../components";

export default function PresetPanel({ presets, style }) {
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
    return(
        <Flex direction="row" align={"center"} style={style}>
            <TDialog
                id="savePresetForm"
                header="Save Preset"
                description=""
                fields={fieldFormat}
                buttonText="Save Preset"
                ></TDialog>
        </Flex>
    );
}
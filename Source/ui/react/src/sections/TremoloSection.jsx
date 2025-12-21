//import Slider from "../components/Slider.jsx";
import { ParameterID, ElementID } from "../utilities/juceBridge.js";
import { Heading } from "@radix-ui/themes";
import { TSlider, TDropdown, TButton, TCheckBox } from "../components";

export default function TremoloSection(data) {
  const presets = data.data.Presets[0];
  return (
    <div>
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

      <TDropdown id={ParameterID.TREMWAVE} options={presets}></TDropdown>

      <Heading>Save preset</Heading>
      <TButton text="Save Preset" id={ElementID.SAVEPRESETBUTTON}></TButton>
      <TCheckBox label="Bypass Tremolo" id={ParameterID.TREMBP}></TCheckBox>
    </div>
  );
}

import Slider from "../components/Slider.jsx";
import { ParameterID } from "../utilities/juceBridge.js";

export default function TremoloSection() {
  return (
    <div>
      <Slider
        id={ParameterID.TREMRATE}
        label="Tremolo Rate"
        min={0}
        max={20}
        step={0.01}
        defaultValue={0.1}
        radius={10}
      />

      <Slider
        id={ParameterID.TREMDEPTH}
        label="Tremolo Depth"
        min={0}
        max={1}
        step={0.01}
        defaultValue={0.5}
      />
    </div>
  );
}

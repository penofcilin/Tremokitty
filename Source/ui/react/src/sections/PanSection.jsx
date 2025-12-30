import { TSlider, TKnob, WaveSelector } from "../components";
import { useState } from "react";
import {
  ParameterID,
  WaveTypes,
  emitButtonEvent,
} from "../utilities/juceBridge.js";

export default function PanSection({ data }) {
  const [panDepth, setPanDepth] = useState(0.5);
  return (
    <Flex>
      <TKnob
        tooltip={{ enabled: true }}
        id={ParameterID.PANDEPTH}
        onChange={setPanDepth}
      />

      <p style={{ pointerEvents: "none", fontSize: "12px" }}>
        {panDepth.toFixed(2)} hz
      </p>
    </Flex>
  );
}

import { useState } from "react";
import { ParameterID, WaveTypes } from "../utilities/juceBridge.js";
import { Box, Flex, Heading, Separator } from "@radix-ui/themes";
import {
  TSlider,
  TDropdown,
  TCheckbox,
  TToggleGroup,
  WaveSelector,
} from "../components";

export default function TremoloSection({ data, style }) {
  const presets = data.Presets[0];
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
      <Flex direction="column" width="250px" style={style}>
        <Box
          p="2"
          display="inline-block"
          style={{
            backgroundColor: "var(--bg-secondary)",
            borderRadius: "14px",
          }}
        >
          <div
            style={{
              backgroundColor: "var(--bg-darkish)",
              borderRadius: "14px 14px 0 0", // rounded top, flat bottom
              padding: "0px 0px",
              height: "30px",
            }}
          >
            <Flex direction="row" align="center">
              <Separator
                size="4"
                my="2"
                style={{
                  width: "60px",
                  height: "2px",
                  backgroundColor: "white",
                  marginLeft: "7px",
                  marginRight: "2px",
                  transform: "translateY(2px)",
                  borderRadius: "50px",
                }}
              />

              <Heading style={{ alignSelf: "center", color: "white" }}>
                Tremolo
              </Heading>

              <Separator
                size="4"
                my="2"
                style={{
                  width: "60px",
                  height: "2px",
                  backgroundColor: "white",
                  transform: "translateY(2px)",
                  marginLeft: "3px",
                }}
              />
            </Flex>
          </div>
          <Heading>Tremolo Rate</Heading>
          <TSlider
            id={ParameterID.TREMRATE}
            min={0}
            max={20}
            step={0.1}
            defaultValue={10}
            size="3"
            variant="soft"
            tooltip={{
              enabled: true,
            }}
          ></TSlider>

          <Heading>Presets</Heading>

          <Heading>Wave Type</Heading>

          <WaveSelector
            id={ParameterID.TREMWAVE}
            value={waveType}
            onChange={setWaveType}
          ></WaveSelector>

          <TCheckbox label="Bypass Tremolo" id={ParameterID.TREMBP}></TCheckbox>
        </Box>
      </Flex>
    </div>
  );
}

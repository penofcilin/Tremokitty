import { useState } from "react";
import {
  ParameterID,
  WaveTypes,
  emitButtonEvent,
} from "../utilities/juceBridge.js";
import { Box, Flex, Heading, Separator } from "@radix-ui/themes";
import { TSlider, TKnob, WaveSelector } from "../components";

export default function TremoloSection({ style }) {
  const [waveType, setWaveType] = useState(WaveTypes[0]);
  const [panDepth, setPanDepth] = useState(0.5);

  return (
    <div>
      <Flex direction="column" width="250px" style={style}>
        <Box
          p="2"
          display="inline-block"
          style={{
            backgroundColor: "var(--bg-secondary)",
            borderRadius: "14px",
            padding: "5px",
            border: "4px solid white",
          }}
        >
          <div
            onClick={() => {
              emitButtonEvent(ParameterID.TREMBP, 1);
              console.log("clicked");
            }}
            onMouseEnter={(e) => {
              e.currentTarget.style.filter = "brightness(1.3)";
            }}
            onMouseLeave={(e) => {
              e.currentTarget.style.filter = "brightness(1)";
            }}
            style={{
              backgroundColor: "var(--bg-darkish)",
              borderRadius: "14px 14px 0 0", // rounded top, flat bottom
              height: "30px",
              cursor: "pointer",
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
          <Heading size="6">Rate</Heading>
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

          <Separator
            style={{
              margin: "10px 0 10px 5px",
              width: "220px",
              height: "3px",
              background: "var(--bg-darkish)",
            }}
          ></Separator>

          <Heading>Depth</Heading>

          <TSlider
            id={ParameterID.TREMDEPTH}
            min={0}
            max={1}
            step={0.01}
            defaultValue={0.7}
            size="3"
            variant="soft"
            tooltip={{
              enabled: true,
            }}
          ></TSlider>
          <Flex
            direction="column"
            style={{
              display: "inline-flex", // keep it tight around contents
              padding: "4px", // space between border and controls
              border: "4px solid white", // the white box outline
              borderRadius: "10px",
              alignItems: "center",
              marginTop: "5px",
              marginRight: "15px",
            }}
            gap={0}
          >
            <Heading>Wave Type</Heading>

            <WaveSelector
              id={ParameterID.TREMWAVE}
              value={waveType}
              onChange={setWaveType}
            ></WaveSelector>
          </Flex>

          <TKnob
            tooltip={{ enabled: true }}
            id={ParameterID.PANDEPTH}
            style={{ marginTop: "5px" }}
            onChange={setPanDepth}
          ></TKnob>
          <p style={{ pointerEvents: "none" }}>
            Pan Depth: {panDepth.toFixed(2)} hz{" "}
          </p>
        </Box>
      </Flex>
    </div>
  );
}

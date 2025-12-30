import { useState } from "react";
import {
  ParameterID,
  WaveTypes,
  emitButtonEvent,
  NoteTypes,
  ElementID,
} from "../utilities/juceBridge.js";
import { Box, Flex, Heading, Separator } from "@radix-ui/themes";
import {
  TSlider,
  TKnob,
  WaveSelector,
  TButton,
  TDropdown,
} from "../components";

export default function TremoloSection({ style }) {
  const [waveType, setWaveType] = useState(WaveTypes[0]);

  return (
    <Flex direction="column" width="255px" style={style}>
      <Box
        style={{
          backgroundColor: "var(--bg-secondary)",
          borderRadius: "14px",
          overflow: "hidden",
        }}
      >
        {/* ================= HEADER BOX ================= */}
        <div
          onClick={() => emitButtonEvent(ParameterID.TREMBP, 1)}
          onMouseEnter={(e) =>
            (e.currentTarget.style.filter = "brightness(1.3)")
          }
          onMouseLeave={(e) => (e.currentTarget.style.filter = "brightness(1)")}
          style={{
            backgroundColor: "var(--bg-darkish)",
            height: "30px",
            cursor: "pointer",
            display: "flex",
            alignItems: "center",
          }}
        >
          <Flex
            direction="row"
            align="center"
            width="100%"
            style={{ transform: "translateX(2px)" }}
          >
            <Separator
              style={{
                width: "75px",
                height: "2px",
                backgroundColor: "white",
                marginLeft: "7px",
                marginRight: "4px",
              }}
            />

            <Heading size="4" style={{ color: "white" }}>
              Tremolo
            </Heading>

            <Separator
              style={{
                width: "75px",
                height: "2px",
                backgroundColor: "white",
                marginLeft: "4px",
              }}
            />
          </Flex>
        </div>

        {/* ================= RATE / DEPTH GRID ================= */}
        <div
          style={{
            display: "grid",
            gridTemplateColumns: "1fr 1px 80px",
            padding: "6px",
          }}
        >
          {/* ---------- RATE (LEFT) ---------- */}
          <Flex
            direction="column"
            gap="2"
            style={{
              paddingRight: "6px",
              justifyContent: "center",
              alignItems: "center",
            }}
          >
            <Heading size="6">Rate</Heading>

            <TSlider
              id={ParameterID.TREMRATE}
              min={0}
              max={20}
              step={0.1}
              defaultValue={10}
              size="3"
              variant="soft"
              tooltip={{ enabled: true }}
            />

            <TButton
              style={{ width: "60px", height: "35px", align: "center" }}
              id={ParameterID.TREMSYNC}
              isToggle={1}
            >
              Sync
            </TButton>

            <TDropdown
              id={ParameterID.TREMSYNCCHOICE}
              defaultValue={NoteTypes[0]}
              buttonStyle={{ width: "135px" }}
              options={NoteTypes} // May need to make this return index of selection
            ></TDropdown>
          </Flex>

          {/* ---------- VERTICAL DIVIDER ---------- */}
          <div
            style={{
              width: "2px",
              backgroundColor: "var(--bg-separator)",
              height: "100%",
              margin: "0 6px",
            }}
          />

          {/* ---------- DEPTH (RIGHT, VERTICAL SLIDER) ---------- */}
          <Flex
            direction="column"
            align="center"
            gap="2"
            style={{ marginLeft: "7px" }}
          >
            <Heading size="6">Depth</Heading>

            <div style={{ height: "110px" }}>
              <TSlider
                id={ParameterID.TREMDEPTH}
                min={0}
                max={1}
                step={0.01}
                defaultValue={0.7}
                orientation="vertical"
                size="3"
                variant="soft"
                tooltip={{ enabled: true }}
              />
            </div>
          </Flex>
        </div>

        {/* ================= WAVE TYPE ================= */}
        <Flex
          direction="column"
          align="center"
          gap="1"
          style={{
            margin: "6px",
            padding: "6px",
            borderRadius: "10px",
          }}
        >
          <Separator
            style={{
              width: "255px",
              height: "2px",
              backgroundColor: "var(--bg-separator)",
            }}
          ></Separator>
          <Heading size="4">Wave Type</Heading>

          <WaveSelector
            id={ParameterID.TREMWAVE}
            value={waveType}
            onChange={setWaveType}
          />
        </Flex>
      </Box>
    </Flex>
  );
}

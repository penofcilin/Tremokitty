import { useState } from "react";
import {
  ParameterID,
  WaveTypes,
  emitButtonEvent,
  NoteTypes,
} from "../utilities/juceBridge.js";
import { Box, Flex, Heading, Separator } from "@radix-ui/themes";
import { TSlider, WaveSelector, TButton, TDropdown } from "../components";

export default function PanSection({ style, bypassed, toggleBypass }) {
  const [waveType, setWaveType] = useState(WaveTypes[0]);
  const [Depth, setDepth] = useState(0.7);
  const [sync, setSync] = useState(false);

  return (
    <Flex
      direction="column"
      width="255px"
      style={style}
      className="panSection bypassable"
      data-bypassed={bypassed ? "" : undefined}
    >
      <Box
        style={{
          backgroundColor: "var(--bg-secondary)",
          borderRadius: "14px",
          overflow: "hidden",
          border: "2px solid black",
        }}
      >
        {/* ================= HEADER BOX ================= */}
        <div
          className="bypassableHeader panHeader"
          onClick={() => {
            emitButtonEvent(ParameterID.PANBP, 1);
            toggleBypass();
          }}
          onMouseEnter={(e) =>
            (e.currentTarget.style.filter = "brightness(1.3)")
          }
          onMouseLeave={(e) => (e.currentTarget.style.filter = "brightness(1)")}
          style={{
            backgroundColor: "var(--bg-darkish)",
            height: "35px",
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
                width: "95px",
                height: "2px",
                backgroundColor: "white",
                marginLeft: "7px",
                marginRight: "4px",
              }}
            />

            <Heading size="5" className="Heading" style={{ color: "white" }}>
              Pan
            </Heading>

            <Separator
              style={{
                width: "95px",
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
          }}
        >
          {/* ---------- RATE (LEFT) ---------- */}
          <Flex
            direction="column"
            gap="1"
            style={{
              paddingRight: "0px",
              justifyContent: "flex-start",
              alignItems: "center",
            }}
          >
            <div
              style={{
                height: "30px",
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
                    width: "40px",
                    height: "2px",
                    marginRight: "6px",
                    backgroundColor: "black",
                  }}
                />

                <Heading className="Heading" size="6">
                  Rate
                </Heading>

                <Separator
                  style={{
                    width: "40px",
                    height: "2px",
                    marginLeft: "6px",
                    backgroundColor: "black",
                  }}
                />
              </Flex>
            </div>

            <Separator
              style={{
                width: "170px",
                height: "2px",
                backgroundColor: "black",
              }}
            />
            <Flex
              direction="column"
              gap="4"
              style={{
                padding: "1px",
                justifyContent: "flex-start",
                alignItems: "center",
              }}
            >
              <TSlider
                id={ParameterID.PANRATE}
                min={0}
                max={20}
                step={0.1}
                defaultValue={10}
                size="3"
                variant="soft"
                tooltip={{ enabled: true }}
                disabled={sync}
                style={{ width: "150px", marginTop: "3px" }}
              />

              <TButton
                style={{ width: "40px", height: "25px", align: "center" }}
                id={ParameterID.PANSYNC}
                clickEvent={() => setSync((prev) => !prev)}
                isToggle={1}
              >
                Sync
              </TButton>

              <TDropdown
                id={ParameterID.PANSYNCCHOICE}
                defaultValue={NoteTypes[0]}
                buttonStyle={{ width: "135px" }}
                disabled={!sync}
                options={NoteTypes}
              ></TDropdown>
            </Flex>
          </Flex>

          {/* ---------- VERTICAL DIVIDER ---------- */}
          <div
            style={{
              width: "2px",
              backgroundColor: "black",
              height: "100%",
            }}
          />

          {/* ---------- DEPTH (RIGHT, VERTICAL SLIDER) ---------- */}
          <Flex
            direction="column"
            align="center"
            gap="0"
            style={{
              justifyContent: "center",
              alignItems: "center",
              marginLeft: "0px",
            }}
          >
            <Heading className="Heading" size="6">
              Depth
            </Heading>
            <Separator
              style={{
                width: "80px",
                height: "2px",
                backgroundColor: "black",
                transform: "translateY(4px)",
              }}
            />

            <Flex
              direction="column"
              style={{
                height: "140px",
                width: "100%",
                marginTop: "7px",
                justifyContent: "center",
                alignItems: "center",
              }}
            >
              <TSlider
                id={ParameterID.PANDEPTH}
                min={0}
                max={1}
                step={0.01}
                defaultValue={0.7}
                orientation="vertical"
                size="1"
                variant="soft"
                onChange={setDepth}
                style={{ height: "100px", marginBottom: "5px" }}
              />
              <Flex
                style={{
                  backgroundColor: "var(--bg-offWhite)",
                  borderRadius: "5px",
                  border: "2px solid var(--outline)",
                  alignItems: "center",
                  justifyContent: "center",
                  padding: "3px",
                  fontSize: "15px",
                  fontFamily: "var(--font-sans)",
                  fontWeight: "bold",
                  marginBottom: "5px",
                }}
                width="50px"
                height="30px"
              >
                {Depth}
              </Flex>
            </Flex>
          </Flex>
        </div>

        {/* ================= WAVE TYPE ================= */}
        <Flex
          direction="column"
          align="center"
          gap="0"
          style={{
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

          <div
            style={{
              height: "35px",
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
                  width: "40px",
                  height: "2px",
                  marginRight: "6px",
                  backgroundColor: "black",
                }}
              />

              <Heading className="Heading" size="6">
                Wave Type
              </Heading>

              <Separator
                style={{
                  width: "40px",
                  height: "2px",
                  marginLeft: "6px",
                  backgroundColor: "black",
                }}
              />
            </Flex>
          </div>

          <WaveSelector
            id={ParameterID.PANWAVE}
            value={waveType}
            onChange={setWaveType}
            style={{
              marginBottom: "8px",
              border: "2px solid var(--bg-primary)",
            }}
          />
        </Flex>
      </Box>
    </Flex>
  );
}

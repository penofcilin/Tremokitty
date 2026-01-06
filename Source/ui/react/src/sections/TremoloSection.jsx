import { useState, useEffect } from "react";
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
  TButton,
  TDropdown,
  WaveSelector,
  Oscilloscope,
} from "../components";
import { toPercentage } from "../Utilities/General.js";

export default function TremoloSection({ style, bypassed, toggleBypass }) {
  const [waveType, setWaveType] = useState(WaveTypes[0]);
  const [tremDepth, setTremDepth] = useState(0.7);
  const [sync, setSync] = useState(false);
  const [lfoPosition, setLfoPosition] = useState(0);

  //LFO Updates
  useEffect(() => {
    const handler = (v) => {
      setLfoPosition(v);
    };

    window.__JUCE__.backend.addEventListener("TremLFOUpdate", handler);

    return () => {
      window.__JUCE__.backend.removeEventListener("TremLFOUpdate", handler);
    };
  }, []);

  return (
    <Flex
      direction="column"
      width="255px"
      style={style}
      className="bypassable tremoloSection"
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
          className="bypassableHeader tremoloHeader"
          onClick={() => {
            emitButtonEvent(ParameterID.TREMBP, 1);
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
                width: "70px",
                height: "2px",
                backgroundColor: "white",
                marginLeft: "7px",
                marginRight: "4px",
              }}
            />

            <Heading size="5" className="Heading" style={{ color: "white" }}>
              Tremolo
            </Heading>

            <Separator
              style={{
                width: "70px",
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
                width: "175px",
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
              <Oscilloscope
                width={110}
                height={90}
                lfoValue={lfoPosition}
                depth={tremDepth}
              ></Oscilloscope>

              <div style={{ display: "flex", flexDirection: "row" }}>
                {sync && (
                  <TDropdown
                    id={ParameterID.TREMSYNCCHOICE}
                    defaultValue={NoteTypes[0]}
                    buttonStyle={{ width: "130px" }}
                    disabled={!sync}
                    options={NoteTypes}
                  ></TDropdown>
                )}

                {!sync && (
                  <TSlider
                    id={ParameterID.TREMRATE}
                    min={0}
                    max={1}
                    skew={0.5}
                    defaultValue={0.5}
                    size="3"
                    variant="soft"
                    tooltip={{ enabled: true }}
                    tooltipMap={(v) => {
                      return (v * 20).toFixed(2) + " hz";
                    }}
                    disabled={sync}
                    style={{ width: "130px", marginTop: "3px" }}
                  />
                )}

                <TButton
                  style={{ width: "40px", height: "25px", align: "center" }}
                  id={ParameterID.TREMSYNC}
                  clickEvent={() => setSync((prev) => !prev)}
                  isToggle={1}
                >
                  Sync
                </TButton>
              </div>
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
                id={ParameterID.TREMDEPTH}
                min={0}
                max={1}
                step={0.01}
                defaultValue={0.7}
                orientation="vertical"
                size="1"
                variant="soft"
                onChange={setTremDepth}
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
                {toPercentage(tremDepth)}
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
            id={ParameterID.TREMWAVE}
            value={waveType}
            onChange={setWaveType}
            style={{
              marginBottom: "8px",
              border: "2px solid var(--bg-primary)",
              "--item-size": "28px",
            }}
          />
        </Flex>
      </Box>
    </Flex>
  );
}

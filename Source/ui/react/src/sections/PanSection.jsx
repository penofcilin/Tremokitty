import { useRef, useEffect, useState } from "react";
import {
  ParameterID,
  WaveTypes,
  emitButtonEvent,
  NoteTypes,
} from "../utilities/juceBridge.js";
import { Box, Flex, Heading, Separator } from "@radix-ui/themes";
import {
  TSlider,
  WaveSelector,
  TButton,
  TDropdown,
  Oscilloscope,
} from "../components";
import { toPercentage } from "../Utilities/General.js";
import { Link } from "lucide-react";

export default function PanSection({
  style,
  bypassed,
  toggleBypass,
  initialData,
  currentState,
}) {
  const [waveType, setWaveType] = useState(WaveTypes[0]);
  const [depth, setDepth] = useState(0.7);
  const [rate, setRate] = useState(0.5);
  const [sync, setSync] = useState(false);
  const [syncChoice, setSyncChoice] = useState(0);
  const [lfoPosition, setLfoPosition] = useState(0);

  const bypassedRef = useRef(false);

  useEffect(() => {
    bypassedRef.current = bypassed;
  }, [bypassed]);

  //LFO updates for oscilloscope
  useEffect(() => {
    const handler = (v) => {
      if (bypassedRef.current) return;
      setLfoPosition(v);
    };

    window.__JUCE__.backend.addEventListener("PanLFOUpdate", handler);

    return () => {
      window.__JUCE__.backend.removeEventListener("PanLFOUpdate", handler);
    };
  }, []);

  const applyState = (data) => {
    setWaveType(data.PANWAVE);

    setDepth(data.PANDEPTH);

    setRate(data.PANRATE);

    setSync(data.PANSYNC);

    setSyncChoice(data.PANSYNCCHOICE);
  };

  //initial ui updates
  useEffect(() => {
    if (!initialData) return;
    applyState(initialData);
  }, [initialData]);

  //Manual UI updates
  useEffect(() => {
    if (currentState) applyState(currentState);
  }, [currentState]);

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
              <Oscilloscope
                width={110}
                height={90}
                lfoValue={lfoPosition}
                rate={rate}
                depth={depth}
                bypassed={bypassed}
                style={{ transform: "rotate(-90deg)" }}
                reverse={true}
              ></Oscilloscope>

              <div style={{ display: "flex", flexDirection: "row" }}>
                <TSlider
                  key={sync ? "sync" : "rate"}
                  id={sync ? ParameterID.PANSYNCCHOICE : ParameterID.PANRATE}
                  min={0}
                  max={sync ? NoteTypes.length - 1 : 10}
                  skew={sync ? 1 : 0.5}
                  step={sync ? 1 : 0.00001}
                  defaultValue={sync ? 0 : 0.5}
                  value={sync ? (syncChoice ?? 0) : rate}
                  onChange={(v) => {
                    if (sync) {
                      setSyncChoice(Math.round(v));
                    } else {
                      setRate(v);
                    }
                  }}
                  size="3"
                  variant="soft"
                  tooltip={{ enabled: true }}
                  tooltipMap={(v) => {
                    if (sync) {
                      return NoteTypes[Math.round(v)] ?? "";
                    }

                    return `${v.toFixed(2)} hz`;
                  }}
                  style={{ width: "130px", marginTop: "3px" }}
                />

                <TButton
                  style={{
                    width: "30px",
                    height: "27px",
                    marginLeft: "5px",
                    marginBottom: "1px",
                    align: "center",
                  }}
                  id={ParameterID.PANSYNC}
                  clickEvent={() => setSync((prev) => !prev)}
                  isToggle={1}
                  value={sync}
                  tooltip="Sync"
                >
                  <Link
                    style={{ strokeWidth: "2.25px" }}
                    width={18}
                    height={18}
                  />
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
                id={ParameterID.PANDEPTH}
                min={0}
                max={1}
                step={0.01}
                defaultValue={0.7}
                value={depth}
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
                {toPercentage(depth)}
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
            onChange={setWaveType}
            value={String(waveType)}
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

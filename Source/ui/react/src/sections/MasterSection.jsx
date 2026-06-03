import { useState, useEffect } from "react";
import {
  ParameterID,
  WaveTypes,
  emitButtonEvent,
  NoteTypes,
  ElementID,
  emitSliderEvent,
} from "../utilities/juceBridge.js";
import {
  TSlider,
  TButton,
  TDropdown,
  WaveSelector,
  Oscilloscope,
  TKnob,
} from "../components";
import { Box, Flex, Heading, Separator } from "@radix-ui/themes";
import { toPercentage } from "../Utilities/General.js";

export default function MasterSection({
  style,
  initialData,
  currentState,
  bypassed,
  toggleBypass,
}) {
  const [gain, setGain] = useState(1);
  const [mix, setMix] = useState(1);

  const applyState = (data) => {
    setGain(data.GAIN);
    setMix(data.MIX);
  };

  //Manual UI updates
  useEffect(() => {
    if (currentState) applyState(currentState);
  }, [currentState]);

  //initial ui updates
  useEffect(() => {
    if (!initialData) return;
    applyState(initialData);
  }, [initialData]);

  return (
    <Flex
      style={{ ...style, alignItems: "center", alignSelf: "center" }}
      direction="column"
    >
      <Box
        style={{
          backgroundColor: "var(--bg-secondary)",
          borderRadius: "14px",
          overflow: "hidden",
          border: "2px solid black",
          padding: "0px",
          width: "173px",
          alignItems: "center",
          transform: "translateX(-6px)",
          marginBottom: "5px",
        }}
        className="bypassable"
        data-bypassed={bypassed ? true : undefined}
      >
        {/* ================= HEADER BOX ================= */}
        <div
          className="bypassableHeader masterHeader"
          onClick={() => {
            emitButtonEvent(ParameterID.MASTERBP, 1);
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
            marginBottom: "7px",
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
                width: "36px",
                height: "2px",
                backgroundColor: "white",
                marginLeft: "7px",
                marginRight: "4px",
              }}
            />

            <Heading size="5" className="Heading" style={{ color: "white" }}>
              Master
            </Heading>

            <Separator
              style={{
                width: "36px",
                height: "2px",
                backgroundColor: "white",
                marginLeft: "4px",
              }}
            />
          </Flex>
        </div>
        {/* CONTROLS */}
        <div className="controls" style={{ marginBottom: "5px" }}>
          {/*Knobs*/}
          <Flex
            style={{
              justifyItems: "center",
              alignItems: "center",
            }}
          >
            <div style={{ marginRight: "15px" }}>
              <TKnob
                id={ParameterID.MIX}
                min={0}
                max={1}
                step={0.001}
                value={mix}
                defaultValue={1}
                onChange={(v) => setMix(v)}
                tooltipMap={(v) => {
                  return toPercentage(v);
                }}
                style={{
                  "--knob-size": "50px",
                }}
                tooltip={"enabled"}
              ></TKnob>
              <Heading
                size="5"
                className="Heading"
                style={{ marginLeft: "5px" }}
              >
                Mix
              </Heading>
            </div>
            <div>
              <TKnob
                id={ParameterID.GAIN}
                defaultValue={1}
                min={0}
                max={2}
                step={0.001}
                skew={0}
                value={gain}
                onChange={(v) => setGain(v)}
                style={{
                  "--knob-size": "50px",
                }}
                tooltip="enabled"
                tooltipMap={`${gain.toFixed(2)} X`}
              />

              <Heading
                style={{ marginLeft: "1px" }}
                size="5"
                className="Heading"
              >
                Gain
              </Heading>
            </div>
          </Flex>
        </div>
      </Box>
    </Flex>
  );
}

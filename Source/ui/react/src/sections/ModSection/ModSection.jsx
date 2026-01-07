import { useState, useEffect } from "react";
import {
  ParameterID,
  WaveTypes,
  emitButtonEvent,
  NoteTypes,
  ElementID,
  emitSliderEvent,
} from "../../utilities/juceBridge.js";
import {
  TSlider,
  TButton,
  TDropdown,
  WaveSelector,
  Oscilloscope,
  TKnob,
} from "../../components";
import { Box, Flex, Heading, Separator } from "@radix-ui/themes";
import { normToSkewed, toPercentage } from "../../Utilities/General.js";
import "./ModSection.css";

export default function ModSection({ style, bypassed, toggleBypass }) {
  const [lfoPosition, setLfoPosition] = useState(0);
  const [waveType, setWaveType] = useState(0);
  const [sync, setSync] = useState(true);
  const [syncedRate, setSyncedRate] = useState(10);
  const [unsyncedRate, setUnsyncedRate] = useState(0);
  const [depth, setDepth] = useState(0);

  //LFO Updates
  useEffect(() => {
    const handler = (v) => {
      setLfoPosition(v);
    };

    window.__JUCE__.backend.addEventListener("ModLFOUpdate", handler);

    return () => {
      window.__JUCE__.backend.removeEventListener("ModLFOUpdate", handler);
    };
  }, []);

  //Sync button click
  const syncButtonClicked = () => {
    setSync((prev) => {
      const next = !prev;

      emitSliderEvent(
        sync ? ParameterID.MODSYNCCHOICE : ParameterID.MODLFORATE,
        next ? syncedRate : unsyncedRate
      );

      return next;
    });
  };

  const handleRateChange = (value) => {
    if (sync) {
      setSyncedRate(value);
    } else {
      setUnsyncedRate(value);
    }
  };

  const activeRate = sync ? syncedRate : unsyncedRate;

  return (
    <Flex
      width="170px"
      height="360px" //"521px"
      style={{ style }}
      data-bypassed={bypassed ? "" : undefined}
      className="modSection bypassable"
      direction="column"
    >
      {/* MOD HEADER */}
      <div
        className="bypassableHeader modHeader"
        onClick={() => {
          emitButtonEvent(ParameterID.MODBP, 1);
          toggleBypass();
        }}
        onMouseEnter={(e) => (e.currentTarget.style.filter = "brightness(1.3)")}
        onMouseLeave={(e) => (e.currentTarget.style.filter = "brightness(1)")}
        style={{
          backgroundColor: "var(--bg-darkish)",
          height: "35px",
          width: "100%",
          cursor: "pointer",
          display: "flex",
          alignItems: "center",
          borderRadius: "11px 11px 0 0",
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
              width: "47px",
              height: "2px",
              backgroundColor: "white",
              marginLeft: "7px",
              marginRight: "4px",
            }}
          />

          <Heading size="5" className="Heading" style={{ color: "white" }}>
            MOD
          </Heading>

          <Separator
            style={{
              width: "47px",
              height: "2px",
              backgroundColor: "white",
              marginLeft: "4px",
              marginRight: "4px",
            }}
          />
        </Flex>
      </div>

      <div className="oscilloscopeDiv">
        <Oscilloscope
          lfoValue={lfoPosition}
          depth={depth}
          rate={sync ? syncedRate : unsyncedRate}
        ></Oscilloscope>
      </div>
      {/* CONTROLS */}
      <div className="controls">
        <TButton
          style={{ padding: "5px", width: "40px", height: "30px" }}
          id={ParameterID.MODSYNC}
          clickEvent={syncButtonClicked}
          isToggle={1}
        >
          Sync
        </TButton>

        {/*Knobs*/}
        <Flex
          style={{
            justifyItems: "center",
            alignItems: "center",
          }}
        >
          <div style={{ marginRight: "15px" }}>
            <TKnob
              id={sync ? ParameterID.MODLFORATE : ParameterID.MODSYNCCHOICE}
              defaultValue={sync ? 1 / 5 : 1 / 11}
              step={sync ? 0.00001 : 1 / (NoteTypes.length - 1)}
              skew={sync ? 0.5 : 0}
              value={activeRate}
              onChange={handleRateChange}
              style={{
                "--knob-size": "50px",
              }}
              tooltip="enabled"
              tooltipMap={
                sync
                  ? (v) => `${(v * 10).toFixed(2)} Hz`
                  : (v) => NoteTypes[v * (NoteTypes.length - 1)]
              }
            />

            <Heading size="5" className="Heading">
              Rate
            </Heading>
          </div>
          <div>
            <TKnob
              id={ParameterID.MODLFODEPTH}
              min={0}
              max={1}
              step={0.001}
              defaultValue={0.7}
              onChange={(v) => setDepth(v)}
              tooltipMap={(v) => {
                return toPercentage(v);
              }}
              style={{
                "--knob-size": "50px",
              }}
              tooltip={"enabled"}
            ></TKnob>
            <Heading size="5" className="Heading">
              Depth
            </Heading>
          </div>
        </Flex>
        <Flex
          style={{
            display: "inline-flex",
            padding: "2px 0px 2px 2px",
            border: "2px solid white",
            borderRadius: "10px",
            alignItems: "center",
            marginLeft: "6px",
            marginTop: "5px",
          }}
        >
          <WaveSelector
            id={ParameterID.MODWAVETYPE}
            value={waveType}
            onChange={setWaveType}
            vertical={false}
            style={{ width: "149px", "--item-size": "35px" }}
          ></WaveSelector>
        </Flex>
      </div>
    </Flex>
  );
}

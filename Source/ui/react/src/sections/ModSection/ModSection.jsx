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
import { Link } from "lucide-react";
import { Box, Flex, Heading, Separator } from "@radix-ui/themes";
import { normToSkewed, toPercentage } from "../../Utilities/General.js";
import "./ModSection.css";

export default function ModSection({
  style,
  bypassed,
  toggleBypass,
  initialData,
  currentState,
}) {
  const [lfoPosition, setLfoPosition] = useState(0);
  const [waveType, setWaveType] = useState(0);
  const [sync, setSync] = useState(true);
  const [syncedRate, setSyncedRate] = useState(10);
  const [unsyncedRate, setUnsyncedRate] = useState(0);
  const [depth, setDepth] = useState(0);

  //LFO Updates
  useEffect(() => {
    const handler = (v) => {
      console.log("Mod LFO update:", v);
      setLfoPosition(v);
    };

    window.__JUCE__.backend.addEventListener("ModLFOUpdate", handler);

    return () => {
      window.__JUCE__.backend.removeEventListener("ModLFOUpdate", handler);
    };
  }, []);

  const applyState = (data) => {
    console.log("applying mod state:", data);

    setWaveType(data.MODWAVETYPE);
    setDepth(data.MODLFODEPTH);
    setSync(data.MODSYNC);
    setSyncedRate(data.MODSYNCCHOICE);
    setUnsyncedRate(data.MODLFORATE);
  };

  // Manual UI updates
  useEffect(() => {
    if (currentState) applyState(currentState);
  }, [currentState]);

  // Initial UI updates
  useEffect(() => {
    if (!initialData) return;
    applyState(initialData);
  }, [initialData]);

  //Sync button click
  const syncButtonClicked = () => {
    setSync((prev) => {
      const next = !prev;

      emitSliderEvent(
        sync ? ParameterID.MODSYNCCHOICE : ParameterID.MODLFORATE,
        next ? syncedRate : unsyncedRate,
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
      height="300px"
      style={{ ...style, alignItems: "center" }}
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

      <div
        className="oscilloscopeDiv"
        style={{ transform: "translateY(-5px)" }}
      >
        <Oscilloscope
          lfoValue={lfoPosition}
          depth={depth}
          rate={sync ? syncedRate : unsyncedRate}
          bypassed={bypassed}
          style={{ height: "100px" }}
        />
      </div>
      {/* CONTROLS */}
      <div className="controls">
        <TButton
          style={{ padding: "5px", width: "30px", height: "27px" }}
          id={ParameterID.MODSYNC}
          clickEvent={() => setSync((prev) => !prev)}
          isToggle={1}
          value={sync}
        >
          <Link style={{ strokeWidth: "2.25px" }} width={18} height={18} />
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
              id={sync ? ParameterID.MODSYNCCHOICE : ParameterID.MODLFORATE}
              defaultValue={sync ? 1 / NoteTypes.length : 1 / 5}
              step={sync ? 1 / (NoteTypes.length - 1) : 0.00001}
              skew={sync ? 0 : 0.5}
              value={activeRate}
              onChange={handleRateChange}
              style={{ "--knob-size": "45px" }}
              tooltip="enabled"
              tooltipMap={
                sync
                  ? (v) => NoteTypes[Math.round(v * (NoteTypes.length - 1))]
                  : (v) => `${(v * 10).toFixed(2)} Hz`
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
              value={depth}
              onChange={setDepth}
              tooltipMap={(v) => toPercentage(v)}
              style={{
                "--knob-size": "45px",
                marginLeft: "5px",
              }}
              tooltip="enabled"
            />
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
            alignContent: "center",
            alignSelf: "center",
            justifyContent: "center",
            marginTop: "5px",
            transform: "translateY(-5px)",
          }}
        >
          <WaveSelector
            id={ParameterID.MODWAVETYPE}
            value={waveType}
            onChange={setWaveType}
            vertical={false}
            style={{ width: "110px", "--item-size": "25px", height: "100%" }}
          ></WaveSelector>
        </Flex>
      </div>
    </Flex>
  );
}

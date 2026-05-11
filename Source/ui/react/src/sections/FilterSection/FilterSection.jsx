import { Box, Flex, Heading, Separator } from "@radix-ui/themes";
import { useState, useEffect } from "react";
import {
  ParameterID,
  WaveTypes,
  emitButtonEvent,
  emitSliderEvent,
  NoteTypes,
} from "../../utilities/juceBridge.js";
import { LowPassIcon, HighPassIcon, BandPassIcon } from "../../assets/icons";
import {
  TSlider,
  WaveSelector,
  TButton,
  TToggleGroup,
  TKnob,
  TTooltip,
  FilterGraphic,
} from "../../components";
import { normToSkewed, toPercentage } from "../../Utilities/General.js";
import "./FilterSection.css";

export default function FilterSection({
  style,
  bypassed,
  toggleBypass,
  initialData,
  currentState,
}) {
  const [filterType, setFilterType] = useState(0);
  const [filterWaveType, setFilterWaveType] = useState(WaveTypes[0]);
  const [sync, setSync] = useState(false);
  const [syncedRate, setSyncedRate] = useState(10);
  const [unsyncedRate, setUnsyncedRate] = useState(0);
  const [cutoff, setCutoff] = useState(1);
  const [resonance, setResonance] = useState(0.7);
  const [modDepth, setModDepth] = useState(0);
  const [modBypassed, setModBypassed] = useState(false);
  const [xyCursorPx, setXyCursorPx] = useState(null);
  const [lfoPosition, setLfoPosition] = useState(0);

  //LFO Updates
  useEffect(() => {
    const handler = (v) => {
      setLfoPosition(v);
    };

    window.__JUCE__.backend.addEventListener("FilterLFOUpdate", handler);

    return () => {
      window.__JUCE__.backend.removeEventListener("FilterLFOUpdate", handler);
    };
  }, []);

  //Mod Bypass
  const toggleModulationBypass = () => {
    if (modBypassed) {
      emitSliderEvent(ParameterID.FILTERMODLEVEL, modDepth);
      setModBypassed(false);
    } else {
      emitSliderEvent(ParameterID.FILTERMODLEVEL, 0);
      setModBypassed(true);
    }
  };

  const applyState = (data) => {
    setFilterType(data.FILTERTYPE);

    setFilterWaveType(data.FILTERWAVE);

    setSync(!!data.FILTERSYNC);

    setSyncedRate(data.FILTERSYNCCHOICE);

    setUnsyncedRate(data.FILTERRATE);

    setCutoff(data.FILTERCUTOFF);

    setResonance(data.FILTERRES);

    setModDepth(data.FILTERMODLEVEL);
  };

  //Initial data loading
  useEffect(() => {
    if (!initialData) {
      console.log("no data");
      return;
    }
    if (typeof initialData !== "object") {
      console.log("Nobject");
      return;
    }

    applyState(initialData);
  }, [initialData]);

  //updateUI event listener
  useEffect(() => {
    if (currentState) applyState(currentState);
  }, [currentState]);

  const activeRate = sync ? syncedRate : unsyncedRate;

  const handleRateChange = (value) => {
    if (sync) {
      setSyncedRate(value);
    } else {
      setUnsyncedRate(value);
    }
  };

  //Sync button click
  const syncButtonClicked = () => {
    setSync((prev) => {
      const next = !prev;

      emitSliderEvent(
        sync ? ParameterID.FILTERSYNCCHOICE : ParameterID.FILTERRATE,
        next ? syncedRate : unsyncedRate,
      );

      return next;
    });
  };

  const cutoffTooltip = (n) => {
    const hz = 20 + n * (20000 - 20);

    return hz >= 1000
      ? `${(hz / 1000).toFixed(2)} kHz`
      : `${Math.round(hz)} Hz`;
  };

  //Filter clicking stuff
  const clamp01 = (v) => Math.min(1, Math.max(0, v));

  const updateXYFromEvent = (e) => {
    const rect = e.currentTarget.getBoundingClientRect();

    const x = clamp01((e.clientX - rect.left) / rect.width);
    const y = clamp01((rect.bottom - e.clientY) / rect.height);

    setXyCursorPx({
      x: e.clientX - rect.left,
      y: e.clientY - rect.top,
    });

    const res = 0.7 + y * (10 - 0.7);

    setCutoff(x);
    setResonance(res);

    emitSliderEvent(ParameterID.FILTERCUTOFF, x);
    emitSliderEvent(ParameterID.FILTERRES, res);
  };

  const onXYPointerDown = (e) => {
    e.preventDefault();
    e.currentTarget.setPointerCapture(e.pointerId);
    updateXYFromEvent(e);
  };

  const onXYPointerMove = (e) => {
    if (!e.currentTarget.hasPointerCapture(e.pointerId)) return;
    updateXYFromEvent(e);
  };

  const onXYPointerUp = (e) => {
    setXyCursorPx(null);
    if (e.currentTarget.hasPointerCapture(e.pointerId)) {
      e.currentTarget.releasePointerCapture(e.pointerId);
    }
  };

  return (
    <Flex
      width="525px"
      height="212px"
      style={{ style }}
      data-bypassed={bypassed ? "" : undefined}
      className="filterSection bypassable"
    >
      <div className="sectionLayout">
        <div className="leftStack">
          {/* Header*/}
          <div
            className="filterHeader bypassableHeader"
            onClick={() => {
              emitButtonEvent(ParameterID.FILTERBP, 1);
              toggleBypass();
            }}
            onMouseEnter={(e) =>
              (e.currentTarget.style.filter = "brightness(1.3)")
            }
            onMouseLeave={(e) =>
              (e.currentTarget.style.filter = "brightness(1)")
            }
          >
            <Flex
              direction="row"
              align="center"
              width="100%"
              style={{ transform: "translateX(2px)" }}
            >
              <Separator
                style={{
                  width: "87px",
                  height: "2px",
                  backgroundColor: "white",
                  marginLeft: "3px",
                  marginRight: "4px",
                }}
              />

              <Heading size="5" className="Heading" style={{ color: "white" }}>
                Filter
              </Heading>

              <Separator
                style={{
                  width: "87px",
                  height: "2px",
                  backgroundColor: "white",
                  marginLeft: "4px",
                  marginRight: "4px",
                }}
              />
            </Flex>
          </div>
          {/*Filter graphic + controls */}
          <Flex
            style={{
              display: "inline-flex",
              padding: "4px",
              border: "3px solid white",
              borderRadius: "10px",
              alignItems: "center",
              marginLeft: "7px",
              marginTop: "5px",
            }}
            direction="column"
            gap={0}
          >
            <div className="filterGraphicWithControls">
              <div
                className="filterXYPad"
                onPointerDown={onXYPointerDown}
                onPointerMove={onXYPointerMove}
                onPointerUp={onXYPointerUp}
                onPointerCancel={onXYPointerUp}
                style={{
                  position: "relative",
                  touchAction: "none",
                  cursor: "grabbing",
                }}
              >
                {/* SVG graphic */}
                <FilterGraphic
                  cutoff={cutoff}
                  resonance={resonance}
                  modDepth={modBypassed ? 0 : modDepth}
                  modRate={sync ? syncedRate : unsyncedRate}
                  filterType={filterType}
                  lfoPosition={lfoPosition}
                />

                {xyCursorPx && (
                  <TTooltip
                    side="top"
                    open={true}
                    delay={0}
                    style={{ border: "2px solid var(--outline)" }}
                    content={
                      <div>
                        <div>Cutoff: {cutoffTooltip(cutoff)}</div>
                        <div>Resy res: {resonance.toFixed(2)}</div>
                      </div>
                    }
                  >
                    <div
                      className="xyTooltipAnchor"
                      style={{
                        position: "absolute",
                        left: xyCursorPx.x,
                        top: xyCursorPx.y,
                        width: "1px",
                        height: "1px",
                        pointerEvents: "none",
                      }}
                    />
                  </TTooltip>
                )}
              </div>
              <TToggleGroup
                id={ParameterID.FILTERTYPE}
                value={String(filterType)}
                onChange={(v) => setFilterType(v)}
                vertical={true}
                style={{ marginTop: "0px", transform: "translateY(-2px)" }}
                options={[
                  {
                    value: "0",
                    label: "Low Pass",
                    icon: <LowPassIcon width={18} height={18} />,
                  },
                  {
                    value: "1",
                    label: "High Pass",
                    icon: <HighPassIcon width={18} height={18} />,
                  },
                  {
                    value: "2",
                    label: "Band Pass",
                    icon: <BandPassIcon width={18} height={18} />,
                  },
                ]}
              ></TToggleGroup>
            </div>
            <div className="filterControlsSection">
              <TSlider
                id={ParameterID.FILTERCUTOFF}
                min={0}
                max={1}
                value={cutoff}
                defaultValue={1}
                tooltip={"enabled"}
                tooltipMap={cutoffTooltip}
                onChange={(v) => setCutoff(v)}
                step={0.00001}
                style={{
                  width: "290px",
                  marginLeft: "3px",
                  transform: "translateX(-9px)",
                }}
                className="cutoffSlider"
              ></TSlider>
              <TKnob
                id={ParameterID.FILTERRES}
                tooltip={"enabled"}
                tooltipMap={(v) => {
                  return v.toFixed(2);
                }}
                value={resonance}
                className="resonanceKnob"
                onChange={(v) => setResonance(v)}
                min={0.7}
                max={10}
                step={0.05}
                defaultValue={0.707}
                style={{
                  "--knob-size": "26px",
                  transform: "translateX(3px)",
                }}
              ></TKnob>
            </div>
            <Flex
              direction="row"
              align="center"
              width="100%"
              style={{ marginLeft: "30px" }}
            >
              <Separator
                style={{
                  width: "90px",
                  height: "2px",
                  backgroundColor: "Black",
                  marginLeft: "7px",
                  marginRight: "4px",
                }}
              />

              <Heading
                size="5"
                className="Heading"
                style={{
                  color: "Black",
                  margin: "0px",
                  padding: "0px",
                  marginTop: "0px",
                }}
              >
                Cutoff
              </Heading>

              <Separator
                style={{
                  width: "90px",
                  height: "2px",
                  backgroundColor: "Black",
                  marginLeft: "4px",
                  marginRight: "4px",
                }}
              />

              <Heading
                className="Heading"
                size="5"
                style={{
                  margin: "0px",
                  marginLeft: "7px",
                  transform: "translateX(13px)",
                }}
              >
                Resy
              </Heading>
            </Flex>
          </Flex>
        </div>

        <div
          className="rightPanel bypassable"
          data-bypassed={modBypassed ? true : undefined}
        >
          <Flex
            style={{
              display: "inline-flex", // keep it tight around contents
              padding: "4px", // space between border and controls
              border: "3px solid white", // the white box outline
              borderRadius: "10px",
              alignItems: "center",
              marginTop: "5px",
            }}
            direction="column"
            gap={0}
          >
            <div className="rightPanelComponents">
              {/* header*/}
              <Flex direction="row" align="center" width="100%">
                <div
                  className="modulationHeader bypassableHeader"
                  onClick={() => {
                    toggleModulationBypass();
                  }}
                  onMouseEnter={(e) =>
                    (e.currentTarget.style.filter = "brightness(1.3)")
                  }
                  onMouseLeave={(e) =>
                    (e.currentTarget.style.filter = "brightness(1)")
                  }
                >
                  <Separator
                    style={{
                      width: "5px",
                      height: "2px",
                      backgroundColor: "white",
                      marginRight: "4px",
                      marginLeft: "3px",
                    }}
                  />
                  <Heading
                    size="5"
                    className="Heading"
                    style={{
                      color: "white",
                      margin: "0px",
                      padding: "0px",
                      marginTop: "0px",
                    }}
                  >
                    Modulation
                  </Heading>
                  <Separator
                    style={{
                      width: "5px",
                      height: "2px",
                      backgroundColor: "white",
                      marginLeft: "4px",
                      marginRight: "4px",
                    }}
                  />
                </div>
              </Flex>
              <TButton
                style={{ padding: "5px" }}
                id={ParameterID.FILTERSYNC}
                clickEvent={syncButtonClicked}
                isToggle={1}
                value={sync}
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
                    id={
                      sync
                        ? ParameterID.FILTERSYNCCHOICE
                        : ParameterID.FILTERRATE
                    }
                    defaultValue={sync ? 3 : 2}
                    max={sync ? NoteTypes.length - 1 : 10}
                    step={sync ? 1 : 0.00001}
                    skew={0}
                    value={activeRate}
                    onChange={handleRateChange}
                    style={{
                      "--knob-size": "45px",
                    }}
                    tooltip="enabled"
                    tooltipMap={
                      sync ? (v) => NoteTypes[v] : (v) => `${v.toFixed(2)} Hz`
                    }
                  />

                  <Heading size="5" className="Heading">
                    Rate
                  </Heading>
                </div>
                <div>
                  <TKnob
                    id={ParameterID.FILTERMODLEVEL}
                    min={0}
                    max={1}
                    step={0.001}
                    value={modDepth}
                    defaultValue={0.7}
                    onChange={(v) => setModDepth(v)}
                    tooltipMap={(v) => {
                      return toPercentage(v);
                    }}
                    style={{
                      "--knob-size": "45px",
                      marginLeft: "5px",
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
                  padding: "2px",
                  border: "2px solid white",
                  borderRadius: "10px",
                  alignItems: "center",
                  marginLeft: "9px",
                }}
              >
                <WaveSelector
                  id={ParameterID.FILTERWAVE}
                  value={String(filterWaveType)}
                  onChange={setFilterWaveType}
                  vertical={false}
                  style={{ width: "110px", "--item-size": "25px" }}
                ></WaveSelector>
              </Flex>
            </div>
          </Flex>
        </div>
      </div>
    </Flex>
  );
}

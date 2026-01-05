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
  FilterGraphic,
} from "../../components";
import { normToSkewed, toPercentage } from "../../Utilities/General.js";
import "./FilterSection.css";

export default function FilterSection({ style, bypassed, toggleBypass }) {
  const [filterType, setFilterType] = useState(0);
  const [filterWaveType, setFilterWaveType] = useState(WaveTypes[0]);
  const [sync, setSync] = useState(true);
  const [syncedRate, setSyncedRate] = useState(10);
  const [unsyncedRate, setUnsyncedRate] = useState(0);
  const [cutoff, setCutoff] = useState(1);
  const [resonance, setResonance] = useState(0);
  const [modDepth, setModDepth] = useState(0);
  const [modBypassed, setModBypassed] = useState(false);

  const [lfoPosition, setLfoPosition] = useState(0);

  useEffect(() => {
    const handler = (v) => {
      setLfoPosition(v);
    };

    window.__JUCE__.backend.addEventListener("FilterLFOUpdate", handler);

    return () => {
      window.__JUCE__.backend.removeEventListener("FilterLFOUpdate", handler);
    };
  }, []);

  const toggleModulationBypass = () => {
    if (modBypassed) {
      emitSliderEvent(ParameterID.FILTERMODLEVEL, modDepth);
      setModBypassed(false);
    } else {
      emitSliderEvent(ParameterID.FILTERMODLEVEL, 0);
      setModBypassed(true);
    }
  };

  const activeRate = sync ? syncedRate : unsyncedRate;

  const handleRateChange = (value) => {
    if (sync) {
      setSyncedRate(value);
    } else {
      setUnsyncedRate(value);
    }
  };

  const syncButtonClicked = () => {
    setSync((prev) => {
      const next = !prev;

      emitSliderEvent(
        sync ? ParameterID.FILTERSYNCCHOICE : ParameterID.FILTERRATE,
        next ? syncedRate : unsyncedRate
      );

      return next;
    });
  };

  const cutoffTooltip = (n) => {
    const min = 20;
    const max = 20000;
    const hz = normToSkewed(n, min, max, 0.35);

    return hz >= 1000
      ? `${(hz / 1000).toFixed(2)} kHz`
      : `${Math.round(hz)} Hz`;
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
                  marginLeft: "7px",
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
              display: "inline-flex", // keep it tight around contents
              padding: "4px", // space between border and controls
              border: "3px solid white", // the white box outline
              borderRadius: "10px",
              alignItems: "center",
              marginLeft: "7px",
              marginTop: "5px",
            }}
            direction="column"
            gap={0}
          >
            <div className="filterGraphicWithControls">
              <FilterGraphic
                cutoff={cutoff}
                resonance={resonance * 8}
                modDepth={modBypassed ? 0 : modDepth}
                filterType={filterType}
                lfoPosition={lfoPosition}
              ></FilterGraphic>
              <TToggleGroup
                id={ParameterID.FILTERTYPE}
                value={filterType}
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
                    label: "Negative Cosine",
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
                  return (v * 10).toFixed(2);
                }}
                className="resonanceKnob"
                onChange={(v) => setResonance(v)}
                min={0}
                max={1}
                step={0.00005}
                defaultValue={0}
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
                    console.log("EY");
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
                        ? ParameterID.FILTERRATE
                        : ParameterID.FILTERSYNCCHOICE
                    }
                    defaultValue={sync ? 1 / 5 : 1 / 3}
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
                    id={ParameterID.FILTERMODLEVEL}
                    min={0}
                    max={1}
                    step={0.001}
                    defaultValue={0.7}
                    onChange={(v) => setModDepth(v)}
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
                  padding: "2px",
                  border: "2px solid white",
                  borderRadius: "10px",
                  alignItems: "center",
                  marginLeft: "9px",
                }}
              >
                <WaveSelector
                  id={ParameterID.FILTERWAVE}
                  value={filterWaveType}
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

import { Box, Flex, Heading, Separator } from "@radix-ui/themes";
import { useState } from "react";
import {
  ParameterID,
  WaveTypes,
  emitButtonEvent,
  NoteTypes,
} from "../../utilities/juceBridge.js";
import { LowPassIcon, HighPassIcon, BandPassIcon } from "../../assets/icons";
import {
  TSlider,
  WaveSelector,
  TButton,
  TDropdown,
  TToggleGroup,
  TKnob,
} from "../../components";
import "./FilterSection.css";

export default function FilterSection({ style, bypassed, toggleBypass }) {
  const [filterType, setFilterType] = useState(0);
  const [filterResonance, setFilterResonance] = useState(0);
  const [filterWaveType, setFilterWaveType] = useState(WaveTypes[0]);
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
              <div className="filterGraphic"></div>
              <TToggleGroup
                id={ParameterID.FILTERTYPE}
                value={filterType}
                onChange={setFilterType}
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
                step={0.1}
                style={{
                  width: "290px",
                  marginTop: "0px",
                  flexShrink: "0",
                  transform: "translateX(-10px)",
                }}
                className="cutoffSlider"
              ></TSlider>
              <TKnob
                id={ParameterID.FILTERRES}
                className="resonanceKnob"
                min={0.7}
                max={10}
                step={0.05}
                defaultValue={0.7}
                style={{
                  "--knob-size": "26px",
                  transform: "translateX(5px)",
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

        <div className="rightPanel">
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
                <Separator
                  style={{
                    width: "5px",
                    height: "2px",
                    backgroundColor: "Black",
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
                  Modulation
                </Heading>

                <Separator
                  style={{
                    width: "5px",
                    height: "2px",
                    backgroundColor: "Black",
                    marginLeft: "4px",
                    marginRight: "4px",
                  }}
                />
              </Flex>
              <TButton style={{ padding: "5px" }} id={ParameterID.FILTERSYNC}>
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
                    id={ParameterID.FILTERRATE}
                    min={0}
                    max={20}
                    step={0.001}
                    defaultValue={10}
                    style={{
                      "--knob-size": "50px",
                    }}
                    tooltip={"enabled"}
                  ></TKnob>
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

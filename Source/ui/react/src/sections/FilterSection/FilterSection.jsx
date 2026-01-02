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
  return (
    <Flex
      width="525px"
      height="208px"
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

          <div className="filterGraphicWithControls">
            <div className="filterGraphic"></div>
            <TToggleGroup
              id={ParameterID.FILTERTYPE}
              value={filterType}
              onChange={setFilterType}
              vertical={true}
              style={{ marginTop: "12px" }}
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
              style={{ width: "300px", marginTop: "5px", marginLeft: "15px" }}
              className="cutoffSlider"
            ></TSlider>
            <TKnob
              id={ParameterID.FILTERRES}
              className="resonanceKnob"
              min={0.7}
              max={10}
              step={0.05}
              defaultValue={0}
            ></TKnob>
          </div>
        </div>
        <div className="rightPanel"></div>
      </div>
    </Flex>
  );
}

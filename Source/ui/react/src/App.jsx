import { useState } from "react";
import {
  TremoloSection,
  PanSection,
  HeaderSection,
  FilterSection,
  ModSection,
} from "./sections";
import { Flex, Grid, Theme } from "@radix-ui/themes";
import { PresetsContext } from "./utilities/PresetsContext";
import "@radix-ui/themes/styles.css";
import "./styles/fonts.css";
import "./styles/theme.css";

//Load initlization data from juce
const data = window.__JUCE__.initialisationData;

function App() {
  const [tremBypassed, setTremBypassed] = useState(false);
  const [panBypassed, setPanBypassed] = useState(false);
  const [filterBypassed, setFilterBypassed] = useState(false);
  const [modBypassed, setModBypassed] = useState(false);
  //document.addEventListener("contextmenu", (e) => e.preventDefault()); //Prevent context menu from opening on right click
  return (
    <Theme
      accentColor="pink"
      grayColor="gray"
      panelBackground="solid"
      scaling="100%"
      radius="full"
    >
      <Flex
        direction="column"
        style={{
          margin: 0,
          padding: 0,
          width: "100%",
          height: "100vh",
          background: "var(--bg-primary)",
        }}
      >
        <PresetsContext.Provider value={data.Presets[0]}>
          <HeaderSection presets={data.Presets[0]} />
        </PresetsContext.Provider>
        <div className="mainGrid">
          <div className="tremolo">
            <TremoloSection
              bypassed={tremBypassed}
              toggleBypass={() => setTremBypassed((prev) => !prev)}
              style={{ marginLeft: "10px", marginTop: "5px" }}
            />
          </div>

          <div className="pan">
            <PanSection
              bypassed={panBypassed}
              toggleBypass={() => setPanBypassed((prev) => !prev)}
              style={{ marginRight: "5px", marginTop: "5px" }}
            />
          </div>

          <div
            style={{ gridColumn: "span 2", marginLeft: "10px" }}
            className="filter"
          >
            <FilterSection
              toggleBypass={() => setFilterBypassed((prev) => !prev)}
            ></FilterSection>
          </div>

          <div
            style={{
              marginTop: "5px",
              marginRight: "5px",
              gridColumn: "3",
              gridRow: "1 / -1",
            }}
          >
            <ModSection
              toggleBypass={() => setModBypassed((prev) => !prev)}
            ></ModSection>
          </div>
        </div>
      </Flex>
    </Theme>
  );
}

export default App;

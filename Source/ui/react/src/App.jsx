import {
  TremoloSection,
  PanSection,
  HeaderSection,
  FilterSection,
  ModSection,
} from "./sections";

import { Flex, Theme } from "@radix-ui/themes";
import { PresetsContext } from "./utilities/PresetsContext";
import {
  parseParams,
  resolveDefaults,
  ParamsProvider,
} from "./Utilities/General";

import "@radix-ui/themes/styles.css";
import "./styles/fonts.css";
import "./styles/theme.css";
import "./styles/App.css";

// --------------------------------------------
// Load initialization data from JUCE
// --------------------------------------------
const data = window.__JUCE__.initialisationData;

// Parse XML -> [{ id, value | undefined }]
const parsedParams = parseParams(data.InitialState);

// Apply JUCE defaults -> { PARAMID: value }
const resolvedParams = resolveDefaults(parsedParams);

function App() {
  return (
    <ParamsProvider initialParams={resolvedParams}>
      <Theme
        accentColor="pink"
        grayColor="gray"
        panelBackground="solid"
        scaling="100%"
        radius="full"
      >
        <Flex
          direction="column"
          className="appBackground"
          style={{
            margin: 0,
            padding: 0,
            width: "100%",
            height: "100vh",
          }}
        >
          {/* Presets */}
          <PresetsContext.Provider value={data.Presets[0]}>
            <HeaderSection presets={data.Presets[0]} />
          </PresetsContext.Provider>

          {/* Main UI */}
          <div className="mainGrid">
            <div className="tremolo">
              <TremoloSection
                style={{ marginLeft: "10px", marginTop: "5px" }}
              />
            </div>

            <div className="pan">
              <PanSection style={{ marginRight: "5px", marginTop: "5px" }} />
            </div>

            <div
              className="filter"
              style={{ gridColumn: "span 2", marginLeft: "10px" }}
            >
              <FilterSection />
            </div>

            <div
              className="mod"
              style={{
                marginTop: "5px",
                gridColumn: "3",
                gridRow: "1 / -1",
              }}
            >
              <ModSection />
            </div>
          </div>
        </Flex>
      </Theme>
    </ParamsProvider>
  );
}

export default App;

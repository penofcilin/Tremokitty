import {
  TremoloSection,
  PanSection,
  HeaderSection,
  FilterSection,
  ModSection,
} from "./sections";
import { useEffect, useState } from "react";
import { Flex, Theme } from "@radix-ui/themes";
import { PresetsContext } from "./Utilities/PresetsContext";
import * as Juce from "../juce/index.js";
import "@radix-ui/themes/styles.css";
import "./styles/fonts.css";
import "./styles/theme.css";
import "./styles/App.css";

//load data from juce
const data = window.__JUCE__.initialisationData;

function App() {
  const [initialState, setInitialState] = useState(null);
  const [currentState, setCurrentState] = useState(null);
  const [bypassed, setBypassed] = useState({
    tremolo: false,
    pan: false,
    filter: false,
  });

  //Get initial data
  useEffect(() => {
    const getState = Juce.getNativeFunction("ProvideState");
    getState().then((result) => {
      const init = typeof result === "string" ? JSON.parse(result) : result;
      setInitialState(init);
      setCurrentState(init);
      console.log("Initial state received from backend:", init);
    });
    return () => {};
  }, []);

  //Bypassing
  useEffect(() => {
    if (!currentState) return;

    setBypassed({
      tremolo: Boolean(currentState.TREMBP),
      pan: Boolean(currentState.PANBP),
      filter: Boolean(currentState.FILTERBP),
    });
  }, [currentState]);

  //UI State update listener
  useEffect(() => {
    const handler = (state) => {
      console.log("Updating ui from parent component, currentState:", state);
      setCurrentState(state);
    };

    window.__JUCE__.backend.addEventListener("UpdateUI", handler);

    return () => {
      window.__JUCE__.backend.removeEventListener("UpdateUI", handler);
    };
  }, []);

  //UI State update listener
  useEffect(() => {
    const handler = (updates) => {
      console.log("Received parameter updates:", updates);

      setCurrentState((prev) => ({
        ...(prev ?? {}),
        ...updates,
      }));
    };

    window.__JUCE__.backend.addEventListener("ParametersChanged", handler);

    return () => {
      window.__JUCE__.backend.removeEventListener("ParametersChanged", handler);
    };
  }, []);

  const presetIndex = initialState?.INITPRESETINDEX ?? 0;

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
        className="appBackground"
        style={{
          margin: 0,
          padding: 0,
          width: "100%",
          height: "100vh",
        }}
      >
        {/* Presets */}
        <PresetsContext.Provider
          value={{
            presets: data.Presets,
            initPresetIndex: presetIndex,
          }}
        >
          <HeaderSection currentState={currentState} />
        </PresetsContext.Provider>

        {/* Main UI */}
        <div className="mainGrid">
          <div className="tremolo">
            <TremoloSection
              initialData={initialState}
              bypassed={bypassed.tremolo}
              currentState={currentState}
              toggleBypass={() =>
                setBypassed((prev) => ({ ...prev, tremolo: !prev.tremolo }))
              }
              style={{ marginLeft: "10px", marginTop: "5px" }}
            />
          </div>

          <div className="pan">
            <PanSection
              initialData={initialState}
              bypassed={bypassed.pan}
              currentState={currentState}
              toggleBypass={() =>
                setBypassed((prev) => ({ ...prev, pan: !prev.pan }))
              }
              style={{ marginRight: "5px", marginTop: "5px" }}
            />
          </div>

          <div
            className="filter"
            style={{ gridColumn: "span 2", marginLeft: "10px" }}
          >
            <FilterSection
              bypassed={bypassed.filter}
              currentState={currentState}
              toggleBypass={() =>
                setBypassed((prev) => ({ ...prev, filter: !prev.filter }))
              }
              initialData={initialState}
            />
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
  );
}

export default App;

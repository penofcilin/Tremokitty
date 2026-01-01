import { useState } from "react";
import { TremoloSection, HeaderSection, PresetPanel } from "./sections";
import { Flex, Theme } from "@radix-ui/themes";
import { PresetsContext } from "./utilities/PresetsContext";
import "@radix-ui/themes/styles.css";
import "./styles/fonts.css";
import "./styles/theme.css";

//Load initlization data from juce
const data = window.__JUCE__.initialisationData;

function App() {
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
          width: "100vw",
          height: "100vh",
          background: "var(--bg-primary)",
        }}
      >
        <PresetsContext.Provider value={data.Presets[0]}>
          <HeaderSection presets={data.Presets[0]} />
        </PresetsContext.Provider>
        <TremoloSection style={{ marginTop: "5px", marginLeft: "5px" }} />
      </Flex>
    </Theme>
  );
}

export default App;

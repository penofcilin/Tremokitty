import { useState } from "react";
import { TremoloSection, HeaderSection, PresetPanel } from "./sections";
import { Flex, Theme } from "@radix-ui/themes";
import { PresetsContext } from "./utilities/PresetsContext";
import "./styles/fonts.css";

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
      fontFamily="sans"
    >
      <Flex
        direction="column"
        style={{
          margin: 0,
          padding: 0,
          width: "100vw",
          height: "100vh",
        }}
      >
        <PresetsContext.Provider value={data.Presets[0]}>
          <HeaderSection presets={data.Presets[0]} />
        </PresetsContext.Provider>
        <TremoloSection
          data={data}
          style={{ marginTop: "0px", marginLeft: "5px" }}
        />
      </Flex>
    </Theme>
  );
}

export default App;

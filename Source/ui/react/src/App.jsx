import { useState } from "react";
import { TremoloSection, HeaderSection, PresetPanel } from "./sections";
import { Flex, Theme, ThemePanel } from "@radix-ui/themes";
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
        <HeaderSection />
        <PresetPanel style={{height: "50px", padding: "0 8px"}}></PresetPanel>
        <TremoloSection data={data} style={{ marginTop: "0px", marginLeft: "5px", }} />
      </Flex>
    </Theme>
  );
}

export default App;

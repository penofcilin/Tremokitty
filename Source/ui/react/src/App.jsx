import { useState } from "react";
import { TremoloSection, HeaderSection } from "./sections";
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
        <TremoloSection data={data} style={{ marginTop: "5px" }} />
      </Flex>
    </Theme>
  );
}

export default App;

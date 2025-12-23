import { useState } from "react";
import TremoloSection from "./sections/TremoloSection.jsx";
import { Theme, ThemePanel } from "@radix-ui/themes";
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
      <div>
        <TremoloSection data={data} />
      </div>
      <ThemePanel />
    </Theme>
  );
}

export default App;

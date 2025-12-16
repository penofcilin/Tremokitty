import { useState } from "react";
import TremoloSection from "./sections/TremoloSection.jsx";

//Load initlization data from juce
const data = window.__JUCE__.initialisationData;
const appID = data.pluginName;

function App() {
  //document.addEventListener("contextmenu", (e) => e.preventDefault()); //Prevent context menu from opening on right click
  return (
    <div>
      <TremoloSection />
    </div>
  );
}

export default App;

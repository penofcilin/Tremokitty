import { useState } from "react";
import * as Juce from "../juce/index.js";
import * as bridge from "./Utilities/juceBridge.js";

//Load initlization data from juce
const data = window.__JUCE__.initialisationData;
const appID = data.pluginName;

let parameterName = 1;

//CPP -> JS 2: Event listening
window.__JUCE__.backend.addEventListener("ExampleEvent", (objectFromCPP) => {
  console.log("object from cpp: " + objectFromCPP);
});

//JS -> CPP option 1: Get a native CPP function from juce
const testNativeFunction = () => {
  console.log("hi!");

  //Call the native function, do something with it's result
  Juce.getNativeFunction("testNativeFunction")(1, 2).then((result) => {
    console.log(result);
  });
};

//JS -> CPP option 2: emit an event to the JUCE backend
const emitJuceEvent = () => {
  parameterName++;
  window.__JUCE__.backend.emitEvent("sliderChanged", {
    emittedCount: parameterName, //json property
  });
};

function App() {
  //document.addEventListener("contextmenu", (e) => e.preventDefault()); //Prevent context menu from opening on right click
  return (
    <div>
      <Card />
    </div>
  );
}

function Card() {
  return (
    <>
      <h1>{appID}</h1>
      <div className="card">
        <button onClick={testNativeFunction}>Call a c++ function</button>
      </div>
      <button onClick={() => bridge.emitSliderEvent(parameterName++, 0.78)}>
        Emit a sliderChanged Event
      </button>
    </>
  );
}

export default App;

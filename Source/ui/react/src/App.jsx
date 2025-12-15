import { useState } from "react";
import * as Juce from "../juce/index.js";
import "./App.css";

//Load initlization data from juce
const data = window.__JUCE__.initialisationData;
const appID = data.pluginName;

let emittedCount = 0;

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
  emittedCount++;
  window.__JUCE__.backend.emitEvent("exampleReactEvent", {
    emittedCount: emittedCount, //json property
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
      <button onClick={emitJuceEvent}>Emit a juce Event</button>
    </>
  );
}

export default App;

//This file will handle the interaction between the juce backend and the react frontend. Any emission of events, any native function calls, anything like that goes through a function in this file. Also contains some general information about CPP backend stuff, like parameterIDs, wavetypes, other elementIDs.

import * as Juce from "../../juce/index.js";

//Use this enum type thing for references, so you don't have to remember the codes
//Ex. emitSliderEvent(bridge.ParameterIDs.TREMRATE, 0.5);
export const ParameterID = Object.freeze({
  GAIN: "GAIN",
  MASTERBP: "MASTERBP",
  PRESETINDEX: "PRESETINDEX",

  TREMRATE: "TREMRATE",
  TREMDEPTH: "TREMDEPTH",
  TREMWAVE: "TREMWAVE",
  TREMSYNCCHOICE: "TREMSYNCCHOICE",
  TREMBP: "TREMBP",
  TREMSYNC: "TREMSYNC",

  PANRATE: "PANRATE",
  PANDEPTH: "PANDEPTH",
  PANWAVE: "PANWAVE",
  PANSYNCCHOICE: "PANSYNCCHOICE",
  PANBP: "PANBP",
  PANSYNC: "PANSYNC",

  FILTERRATE: "FILTERRATE",
  FILTERMODLEVEL: "FILTERMODLEVEL",
  FILTERCUTOFF: "FILTERCUTOFF",
  FILTERRES: "FILTERRES",
  FILTERWAVE: "FILTERWAVE",
  FILTERTYPE: "FILTERTYPE",
  FILTERSYNCCHOICE: "FILTERSYNCCHOICE",
  FILTERBP: "FILTERBP",
  FILTERSYNC: "FILTERSYNC",

  MODLFORATE: "MODLFORATE",
  MODLFODEPTH: "MODLFODEPTH",
  MODWAVETYPE: "MODWAVETYPE",
  MODCHOICE: "MODCHOICE",
  LASTMODDEDPARAM: "LASTMODDEDPARAM",
  MODSYNCCHOICE: "MODSYNCCHOICE",
  MODPARAMPRIORVALUE: "MODPARAMPRIORVALUE",
  MODRESETSWITCH: "MODRESETSWITCH",
  MODBP: "MODBP",
  MODSYNC: "MODSYNC",
});

export const ElementID = Object.freeze({
  SAVEPRESETBUTTON: "SAVEPRESETBUTTON",
  OPENPRESETFOLDERBUTTON: "OPENPRESETFOLDERBUTTON",
  NEXTPRESETBUTTON: "NEXTPRESETBUTTON",
  PREVIOUSPRESETBUTTON: "PREVIOUSPRESETBUTTON",
});

export const WaveTypes = Object.freeze([
  "Sine",
  "Cosine",
  "NegativeCosine",
  "Saw",
  "SawDown",
  "Square",
  "InverseSquare",
]);

//Passes a string to cpp, calls function "sliderChanged" within editor
export function emitSliderEvent(sliderID, newValue) {
  window.__JUCE__.backend.emitEvent("SliderChanged", {
    sliderID: sliderID,
    newValue: newValue,
  });
}

//Passes an integer to cpp, calls dropdownCommit within editor
export function emitDropdownEvent(dropdownID, newValue) {
  window.__JUCE__.backend.emitEvent("DropdownCommit", {
    dropdownID: dropdownID,
    newValue: newValue,
  });
}

//Passes button id, and an integer(bool) for checking if it's a checkbox. CPP should handle toggling.
export function emitButtonEvent(buttonID, isCheckBox) {
  window.__JUCE__.backend.emitEvent("ButtonClicked", {
    buttonID: buttonID,
    isCheckBox: isCheckBox,
  });
}

export function emitFormEvent(formID, data) {
  window.__JUCE__.backend.emitEvent("FormSubmitted", {
    formID: formID,
    data: data,
  });
}

export function emitTogglegroupEvent(togglegroupID, newValue) {
  window.__JUCE__.backend.emitEvent("TogglegroupChanged", {
    togglegroupID: togglegroupID,
    newValue: newValue,
  });
}

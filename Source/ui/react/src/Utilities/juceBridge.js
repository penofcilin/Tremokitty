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

export const PARAM_DEFAULTS = {
  // =====================
  // Master / Global
  // =====================
  GAIN: 1.0,
  MASTERBP: false,
  PRESETINDEX: 0,

  // =====================
  // Tremolo
  // =====================
  TREMRATE: 0.1,
  TREMDEPTH: 0.5,
  TREMWAVE: 0, // choice index
  TREMSYNCCHOICE: 3,
  TREMBP: false,
  TREMSYNC: false,

  // =====================
  // Panner
  // =====================
  PANRATE: 7.5,
  PANDEPTH: 0.0,
  PANWAVE: 0,
  PANSYNCCHOICE: 3,
  PANBP: false,
  PANSYNC: false,

  // =====================
  // Filter
  // =====================
  FILTERRATE: 0.0,
  FILTERMODLEVEL: 0.0,
  FILTERCUTOFF: 0.9,
  FILTERRES: 1.0 / Math.sqrt(2.0), // ≈ 0.70710678
  FILTERWAVE: 0,
  FILTERTYPE: 0,
  FILTERSYNCCHOICE: 3,
  FILTERBP: false,
  FILTERSYNC: false,

  // =====================
  // Mod LFO
  // =====================
  MODLFORATE: 0.0,
  MODLFODEPTH: 0.0,
  MODWAVETYPE: 0,
  MODCHOICE: 0,
  LASTMODDEDPARAM: 0,
  MODSYNCCHOICE: 3,
  MODPARAMPRIORVALUE: 0.0,
  MODRESETSWITCH: true,
  MODBP: false,
  MODSYNC: false,
};

export const ElementID = Object.freeze({
  SAVEPRESETBUTTON: "SAVEPRESETBUTTON",
  OPENPRESETFOLDERBUTTON: "OPENPRESETFOLDERBUTTON",
  NEXTPRESETBUTTON: "NEXTPRESETBUTTON",
  PREVIOUSPRESETBUTTON: "PREVIOUSPRESETBUTTON",
  RESETPRESETBUTTON: "RESETPRESETBUTTON",
});

export const WaveTypes = Object.freeze([
  "Sine",
  "Cosine",
  "NegativeCosine",
  "Saw",
  "SawDown",
  "Square",
  "InverseSquare",
  "Random",
]);

export const NoteTypes = Object.freeze([
  "Whole",
  "Half",
  "Dotted Half",
  "Quarter",
  "Dotted Quarter",
  "Triplet Quarter",
  "Eighth",
  "Dotted Eighth",
  "Triplet Eighth",
  "Sixteenth",
  "Dotted Sixteenth",
  "Triplet Sixteenth",
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

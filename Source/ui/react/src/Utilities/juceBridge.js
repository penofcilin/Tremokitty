//This file will handle the interaction between the juce backend and the react frontend. Any emission of events, any native function calls, anything like that goes through a function in this file.

/*
List of all parameter IDs:

ID  -  Readable name  - integer identifier

GAIN — Gain — 1
MASTERBP — Master Bypass — 2
PRESETINDEX — Preset Index — 3

TREMRATE — Tremolo Rate — 4
TREMDEPTH — Tremolo Depth — 5
TREMWAVE — Tremolo Modulation Waveform — 6
TREMSYNCCHOICE — Tremolo Sync Rate Choice — 7
TREMBP — Tremolo Bypass — 8
TREMSYNC — Tremolo Sync — 9

PANRATE — Pan Rate — 10
PANDEPTH — Pan Depth — 11
PANWAVE — Pan Mod Waveform — 12
PANSYNCCHOICE — Pan Sync Rate Choice — 13
PANBP — Pan Bypass — 14
PANSYNC — Pan Sync — 15

FILTERRATE — Filter Rate — 16
FILTERMODLEVEL — Filter Mod Level — 17
FILTERCUTOFF — Filter Cutoff — 18
FILTERRES — Filter Resonance — 19
FILTERWAVE — Filter Mod Waveform — 20
FILTERTYPE — Filter Type — 21
FILTERSYNCCHOICE — Filter Sync Rate Choice — 22
FILTERBP — Filter Bypass — 23
FILTERSYNC — Filter Sync — 24

MODLFORATE — Mod LFO Rate — 25
MODLFODEPTH — Mod LFO Depth — 26
MODWAVETYPE — Mod LFO Wave Type — 27
MODCHOICE — Mod LFO Parameter Choice — 28
LASTMODDEDPARAM — The String Name of the last parameter that was modded — 29
MODSYNCCHOICE — Mod Sync Rate Choice — 30
MODPARAMPRIORVALUE — Modded Parameter Pre-modded Value — 31
MODRESETSWITCH — Modded Param Reset Switch — 32
MODBP — Mod LFO Bypass — 33
MODSYNC — Mod Sync — 34

*/
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

//Passes a string to cpp, calls function "sliderChanged" within editor
export function emitSliderEvent(sliderID, newValue) {
  console.log(
    "emitting slider event with id: " + sliderID + " and val " + newValue
  );
  window.__JUCE__.backend.emitEvent("SliderChanged", {
    sliderID: sliderID,
    newValue: newValue,
  });
}

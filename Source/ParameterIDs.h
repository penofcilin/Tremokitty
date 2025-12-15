#pragma once
#ifndef PARAMETERIDS_h
#define PARAMETERIDS_h

inline constexpr int PARAMETER_COUNT = 34;

enum class ParameterIDs : int
{
    // Global
    GAIN = 1,
    MASTERBP = 2,
    PRESETINDEX = 3,

    // Tremolo
    TREMRATE = 4,
    TREMDEPTH = 5,
    TREMWAVE = 6,
    TREMSYNCCHOICE = 7,
    TREMBP = 8,
    TREMSYNC = 9,

    // Panner
    PANRATE = 10,
    PANDEPTH = 11,
    PANWAVE = 12,
    PANSYNCCHOICE = 13,
    PANBP = 14,
    PANSYNC = 15,

    // Filter
    FILTERRATE = 16,
    FILTERMODLEVEL = 17,
    FILTERCUTOFF = 18,
    FILTERRES = 19,
    FILTERWAVE = 20,
    FILTERTYPE = 21,
    FILTERSYNCCHOICE = 22,
    FILTERBP = 23,
    FILTERSYNC = 24,

    // Mod LFO
    MODLFORATE = 25,
    MODLFODEPTH = 26,
    MODWAVETYPE = 27,
    MODCHOICE = 28,
    LASTMODDEDPARAM = 29,
    MODSYNCCHOICE = 30,
    MODPARAMPRIORVALUE = 31,
    MODRESETSWITCH = 32,
    MODBP = 33,
    MODSYNC = 34
};

inline static constexpr const char* parameterIDStrings[] =
{
    "",                 // index 0 — unused (since your enum starts at 1)
    "GAIN",             // 1
    "MASTERBP",         // 2
    "PRESETINDEX",      // 3
    "TREMRATE",         // 4
    "TREMDEPTH",        // 5
    "TREMWAVE",         // 6
    "TREMSYNCCHOICE",   // 7
    "TREMBP",           // 8
    "TREMSYNC",         // 9
    "PANRATE",          // 10
    "PANDEPTH",         // 11
    "PANWAVE",          // 12
    "PANSYNCCHOICE",    // 13
    "PANBP",            // 14
    "PANSYNC",          // 15
    "FILTERRATE",       // 16
    "FILTERMODLEVEL",   // 17
    "FILTERCUTOFF",     // 18
    "FILTERRES",        // 19
    "FILTERWAVE",       // 20
    "FILTERTYPE",       // 21
    "FILTERSYNCCHOICE", // 22
    "FILTERBP",         // 23
    "FILTERSYNC",       // 24
    "MODLFORATE",       // 25
    "MODLFODEPTH",      // 26
    "MODWAVETYPE",      // 27
    "MODCHOICE",        // 28
    "LASTMODDEDPARAM",  // 29
    "MODSYNCCHOICE",    // 30
    "MODPARAMPRIORVALUE", // 31
    "MODRESETSWITCH",   // 32
    "MODBP",            // 33
    "MODSYNC"           // 34
};

#endif /*PARAMETERIDS*/

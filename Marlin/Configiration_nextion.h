#ifndef CONFIGURATION_NEXTION_H
#define CONFIGURATION_NEXTION_H

//
// Nextion LCD. (P.S. if you wanna use this display, change #define SERIAL_PORT 1)
//
#define NEXTION_LCD

//=============================================================================
//======================== Nextion / Screen Selection =========================
//=====================  (Screen type support functions)  =====================
//=============================================================================

#include "src/core/macros.h"

#if ENABLED(NEXTION_LCD)

    #define NEXTION_DEBUG
    //#define NEXTION_TIME
    #define SD_LINES 6

    //visual elements id's
    #define tExtruder1A_id 28
    #define tExtruder1T_id 29
    #if EXTRUDERS > 1
    #define tExtruder2A_id 38
    #define tExtruder2T_id 39
    #endif
    #define tBedT_id 6
    #define tBedA_id 5
    #define tX_id 16
    #define tY_id 17
    #define tZ_id 18
    #define tMessage_id 7
    #define tFan_id 21
    #define tLayers_id 21
    #define tETA_id 19
    #define tPercentage_id 8
    #define btPower_id 5
    #define btLight_id 6
    #define pPower_id 39
    #define pLight_id 30
    #define vaPower_id 8
    #define vaLight_id 9
    #define pHome_id 35
    #define pExtruding_id 34
    #define tmSS_id 37
    #define vaCounter_id 38

    #define pEmpty_id 20
    #define pPower_e_id 19
    #define pLight_e_id 18
    #define pExtruding_e_id 23
    #define pHome_e_id 24

    //page id's
    #define main_page 2
    #define printing_page 3
    #define menu_page 4
    #define settings_page 6
    #define SD_page 5
    //#define movement_page 5
    //#define gcode_page 6
    #define screenSaver_page 7

#endif

//=============================================================================
//=========================END NEXTION CONFIGURATION===========================
//=============================================================================

#endif
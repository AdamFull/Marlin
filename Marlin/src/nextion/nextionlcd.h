#pragma once

#include "../inc/MarlinConfig.h"
#include "../../Configiration_nextion.h"

#if ENABLED(NEXTION_LCD)

    #include "../Marlin.h"

    #if HAS_BUZZER
        #include "../libs/buzzer.h"
    #endif

class NextionUI
{
public:
    NextionUI()
    {

    }

    static void init();
    static void update();
    static void set_alert_status_P(PGM_P message);
    static void set_status_P(PGM_P const message, const int8_t level=0);
    static inline void status_printf_P(const uint8_t level, PGM_P const fmt, ...) { UNUSED(level); UNUSED(fmt); }
    static void reset_status();

    static bool has_status();

    //static void update_sd();

    static uint8_t alert_level; // Higher levels block lower levels
    static inline void reset_alert_level() { alert_level = 0; }
    
private:
    static void processBuffer(const char* receivedString);
    static void ProcessPage(char * inputString, uint8_t receivedBytes);
    static void processMessage(const char * message);
    static void SerialEvent();

};

extern NextionUI ui;

#define LCD_MESSAGEPGM(x)      ui.set_status_P(PSTR(x))
#define LCD_ALERTMESSAGEPGM(x) ui.set_alert_status_P(PSTR(x))

#endif
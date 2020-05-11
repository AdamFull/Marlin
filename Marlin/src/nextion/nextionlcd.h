#pragma once

#include "../inc/MarlinConfigPre.h"
#include "../../Configiration_nextion.h"

#if ENABLED(NEXTION_LCD)

    #include "../MarlinCore.h"

    #if HAS_BUZZER
        #include "../libs/buzzer.h"
    #endif

    #if ENABLED(SDSUPPORT)
      #include "../sd/cardreader.h"
      #define IFSD(A,B) (A)
    #else
        #define IFSD(A,B) (B)
    #endif

    enum LCDViewAction : uint8_t {
    LCDVIEW_NONE,
    LCDVIEW_REDRAW_NOW,
    LCDVIEW_CALL_REDRAW_NEXT,
    LCDVIEW_CLEAR_CALL_REDRAW,
    LCDVIEW_CALL_NO_REDRAW
  };

class NextionUI
{
public:
    NextionUI()
    {

    }

    static void init();
    static void update();
    static void kill_screen(PGM_P const lcd_error, PGM_P const lcd_component);
    static void set_alert_status_P(PGM_P const message);
    static void set_status(const char* const message, const bool persist=false);
    static void set_status_P(PGM_P const message, const int8_t level=0);
    static inline void status_printf_P(const uint8_t level, PGM_P const fmt, ...) { UNUSED(level); UNUSED(fmt); }
    static void reset_status(const bool no_welcome=false);
    //static void return_to_status();

    static bool has_status();

    //static void update_sd();
    static void abort_print();

    static uint8_t alert_level; // Higher levels block lower levels
    static inline void reset_alert_level() { alert_level = 0; }

    static void update_sd(bool is_inserted, bool forcibly=false);
    static void read_sd();
    static void read_sd_again();

    static void update_buttons() {} //check

    #if ENABLED(LCD_BED_LEVELING) && EITHER(PROBE_MANUALLY, MESH_BED_LEVELING)
        static bool wait_for_bl_move;
    #else
        static constexpr bool wait_for_bl_move = false;
    #endif

    static LCDViewAction lcdDrawUpdate;
    FORCE_INLINE static bool should_draw() { return bool(lcdDrawUpdate); }
    FORCE_INLINE static void refresh(const LCDViewAction type) { lcdDrawUpdate = type; }
    FORCE_INLINE static void refresh() { refresh(LCDVIEW_CLEAR_CALL_REDRAW); }
    
private:
    static void processBuffer(const char* receivedString);
    static void ProcessPage(char * inputString, uint8_t receivedBytes);
    static void processMessage(const char * message);
    static void SerialEvent();

    static bool isPrintingFromMedia();
    static bool isPrinting();
    static bool isMoving();

    #if HAS_PRINT_PROGRESS
        static uint8_t get_progress();
    #endif

};

extern NextionUI ui;

#define LCD_MESSAGEPGM_P(x)      ui.set_status_P(x)
#define LCD_ALERTMESSAGEPGM_P(x) ui.set_alert_status_P(x)

#define LCD_MESSAGEPGM(x)        LCD_MESSAGEPGM_P(GET_TEXT(x))
#define LCD_ALERTMESSAGEPGM(x)   LCD_ALERTMESSAGEPGM_P(GET_TEXT(x))

#endif
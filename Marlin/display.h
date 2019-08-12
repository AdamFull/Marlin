#ifndef DISPLAY_H
#define DISPLAY_H

#include "MarlinConfig.h"

#if ENABLED(NEXTION_LCD) && !ENABLED(ULTRA_LCD)
    #include "nextionlcd.h"
#elif ENABLED(ULTRA_LCD) && !ENABLED(NEXTION_LCD)
    #include "ultralcd.h"
#else

    #include "Marlin.h"

    inline void lcd_init() {}
    inline bool lcd_detected() { return true; }
    inline void lcd_update() {}
    inline void lcd_setalertstatusPGM(const char* message) { UNUSED(message); }

    inline bool lcd_hasstatus() { return true; }
    inline void lcd_setstatus(const char* message, const bool persist=false) {}
    inline void lcd_setstatusPGM(const char* message, const int8_t level=0) {}
    inline void lcd_reset_alert_level() {}
    inline void lcd_reset_status() {}

    uint8_t lcdDrawUpdate;
    inline void lcd_refresh() { lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW; }

    #define LCD_MESSAGEPGM(x)      lcd_setstatusPGM(PSTR(x))
    #define LCD_ALERTMESSAGEPGM(x) lcd_setalertstatusPGM(PSTR(x))

#endif
    
#endif
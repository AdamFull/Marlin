#ifndef DISPLAY_H
#define DISPLAY_H

#include "MarlinConfig.h"

#if ENABLED(NEXTION_LCD) && !ENABLED(ULTRA_LCD)
    #include "nextionlcd.h"
#elif ENABLED(ULTRA_LCD) && !ENABLED(NEXTION_LCD)
    #include "ultralcd.h"
#else

    #include "ultralcd.h"
    
    /*#include "Marlin.h"

    inline void lcd_init() {}
    inline bool lcd_detected() { return true; }
    inline void lcd_update() {}
    inline void lcd_setalertstatusPGM(const char* message) { UNUSED(message); }

    constexpr bool lcd_wait_for_move = false;

    inline void lcd_refresh() {}
    inline bool lcd_hasstatus() { return false; }
    inline void lcd_setstatus(const char* const message, const bool persist=false) { UNUSED(message); UNUSED(persist); }
    inline void lcd_setstatusPGM(const char* const message, const int8_t level=0) { UNUSED(message); UNUSED(level); }
    inline void lcd_status_printf_P(const uint8_t level, const char * const fmt, ...) { UNUSED(level); UNUSED(fmt); }
    inline void lcd_reset_alert_level() {}
    inline void lcd_reset_status() {}

    #define LCD_MESSAGEPGM(x)      lcd_setstatusPGM(PSTR(x))
    #define LCD_ALERTMESSAGEPGM(x) lcd_setalertstatusPGM(PSTR(x))*/

#endif
    
#endif
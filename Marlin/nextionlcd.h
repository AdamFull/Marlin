#ifndef NEXTIONLED_H
#define NEXTIONLED_H

#include "MarlinConfig.h"

#if ENABLED(NEXTION_LCD)
    void lcd_init();
    bool lcd_detected();
    void lcd_update();
    void lcd_setalertstatusPGM(const char* message);
#else
    inline void lcd_init() {}
    inline bool lcd_detected() { return true; }
    inline void lcd_update() {}
    inline void lcd_setalertstatusPGM(const char* message) { UNUSED(message); }
#endif

#if ENABLED(NEXTION_LCD)

    #include "Marlin.h"

    bool lcd_hasstatus();
    void lcd_setstatus(const char* message, const bool persist=false);
    void lcd_setstatusPGM(const char* message, const int8_t level=0);
    void lcd_setalertstatusPGM(const char* message);
    void lcd_reset_status();
    void lcd_reset_alert_level();

    extern uint8_t lcdDrawUpdate;
    inline void lcd_refresh() { lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW; }

    #define LCD_MESSAGEPGM(x)      lcd_setstatusPGM(PSTR(x))
    #define LCD_ALERTMESSAGEPGM(x) lcd_setalertstatusPGM(PSTR(x))

#endif


#endif
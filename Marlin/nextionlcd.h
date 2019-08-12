#ifndef NEXTIONLCD_H
#define NEXTIONLCD_H

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
    void lcd_reset_alert_level();
    void lcd_reset_status();

    extern uint8_t lcdDrawUpdate;
    inline void lcd_refresh() { lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW; }

    #if HAS_BUZZER
        void lcd_buzz(const long duration, const uint16_t freq);
    #endif

    #if ENABLED(SHOW_BOOTSCREEN)
        void lcd_bootscreen();
    #endif

    #define LCD_UPDATE_INTERVAL 400

    #if ENABLED(FILAMENT_LCD_DISPLAY) && ENABLED(SDSUPPORT)
        extern millis_t previous_lcd_status_ms;
    #endif

    //bool lcd_blink();

    inline void lcd_status_printf_P(const uint8_t level, const char * const fmt, ...) { UNUSED(level); UNUSED(fmt); }
    inline void status_printf(uint8_t level, const char *status, ...) { UNUSED(level); UNUSED(status); }

#endif

    #define LCD_MESSAGEPGM(x)      lcd_setstatusPGM(PSTR(x))
    #define LCD_ALERTMESSAGEPGM(x) lcd_setalertstatusPGM(PSTR(x))

#endif
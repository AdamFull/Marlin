#include "MarlinConfig.h"

#if ENABLED(NEXTION_LCD)

    #include "nextionlcd.h"
    #include "Marlin.h"
    #include "language.h"
    #include "cardreader.h"
    #include "temperature.h"
    #include "planner.h"
    #include "stepper.h"
    #include "configuration_store.h"
    #include "utility.h"
    #include "parser.h"

    void lcd_init() {}
    bool lcd_detected() { return true; }
    void lcd_update() {}
    void lcd_setalertstatusPGM(const char* message) {}

    bool lcd_hasstatus() { return true; }
    void lcd_setstatus(const char* message, const bool persist=false) {}
    void lcd_setstatusPGM(const char* message, const int8_t level=0) {}
    void lcd_reset_alert_level() {}
    void lcd_reset_status() {}

    uint8_t lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW;

#endif
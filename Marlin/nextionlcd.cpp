#include "MarlinConfig.h"

#if ENABLED(NEXTION_LCD)

    #include "nextionlcd.h"
    #include "nextionlcdelements.h"
    #include "Marlin.h"

    nextionlcdelements interface;

    #if ENABLED(NEXTION_TIME)
        #include "TimeLib.h"
    #endif

    const byte MAX_MESSAGE_LENGTH = 255;
    char lcd_status_message[MAX_MESSAGE_LENGTH + 1];
    uint8_t lcd_status_update_delay = 1, // First update one loop delayed
        lcd_status_message_level;

    #if HAS_BUZZER && DISABLED(LCD_USE_I2C_BUZZER)
        #include "buzzer.h"
    #endif

    #include "printcounter.h"

    #if ENABLED(PRINTCOUNTER)
        #include "duration_t.h"
    #endif

    #if ENABLED(BLTOUCH)
        #include "endstops.h"
    #endif

    #if ENABLED(AUTO_BED_LEVELING_UBL)
        #include "ubl.h"
    #elif HAS_ABL
        #include "planner.h"
    #elif ENABLED(MESH_BED_LEVELING) && ENABLED(LCD_BED_LEVELING)
        #include "mesh_bed_leveling.h"
    #endif

    #if ENABLED(FWRETRACT)
        #include "fwretract.h"
    #endif

    #if ENABLED(POWER_LOSS_RECOVERY)
        #include "power_loss_recovery.h"
    #endif

    void lcd_init() 
    {
        interface.setInitStatus(nexInit());
        lcd_update();
        interface.setStarted();
    }

    bool lcd_detected() { return interface.getInitStatus(); }
    void lcd_update() {}
    void lcd_setalertstatusPGM(const char* message) {}

    bool lcd_hasstatus() { return (lcd_status_message[0] != '\0'); }
    void lcd_setstatus(const char* message, const bool persist) {}
    void lcd_setstatusPGM(const char* message, const int8_t level) {}
    void lcd_reset_alert_level() {}
    void lcd_reset_status() {}

    uint8_t lcdDrawUpdate = LCDVIEW_CLEAR_CALL_REDRAW;

#endif
#pragma once

#include "../inc/MarlinConfig.h"
#include "../../Configiration_nextion.h"

#if ENABLED(NEXTION_LCD)

    #define num_of_lines 6

    #include "NextionLib/Nextion.h"

    #if ENABLED(NEXTION_TIME)
        #include "Wire.h"
        #include "TimeLib.h"
    #endif

    class nextionlcdelements    //Nextion visual components control
    {
    public:
        void setExtruderActual();
        #if ENABLED(MESH_BED_LEVELING)
        void setExtruderTarget();
        #else
        inline void setExtruderTarget() {}
        #endif

        void setBedTarget(char *temp);
        void setBedActual();

        inline void enableBed() { }

        #if ENABLED(MESH_BED_LEVELING)
        void setXPos();
        void setYPos();
        void setZPos();
        #else
        inline void setXPos() {}
        inline void setYPos() {}
        inline void setZPos() {}
        #endif

        void setMessage(const char * inStr);

        void setFan(char *temp);

        //void setTLayers(const char * inStr);
        void setTETA(const char * inStr);
        void setTPercentage(uint8_t value);

        //void setPower(bool status);
        //void setCaseLight(bool status);

        #if ENABLED(NEXTION_TIME)
	        void setTime(int year, int month, int day, int hour, int minute);
	        void setTime();
        #else
            inline void setTime(int year, int month, int day, int hour, int minute) {}
	        inline void setTime() {}
        #endif

        inline void setInitStatus(bool value) { init_status = value; }
        inline bool getInitStatus() { return init_status; }

        void setPage(int page);
	    int getPage();
	    void resetPageChanged();
	    bool pageChanged();

        inline void setSDState(bool enabled) { pSD.setPic(enabled ? 19 : 20); }
        void update_sd(char files_list[64][27], uint16_t files_less, uint16_t files_count); //SD card scroll list

        inline void setHasFan(bool state) { vaHasFan.setValue(state); }

        inline void setExtruders(uint32_t value) { vaExtruders.setValue(value); }
        inline void setIsPaused(bool value) { vaPaused.setValue(value ? 1 : 0); }

        inline void setElapsed(const char* value) { tElapsed.setText(value); }
        inline void setETA(const char* value) { tETA.setText(value); }

        void animate();

        void setIsPrinting(bool status);
        void setProgress(uint8_t perc);
	    void setIsHomed(bool status);
	    bool getIsHomed();

        void setStarted();
	    bool isStarted();

        void set_allert_screen();
        void set_allert_message(const char* message);

        char* XPortMsg(char* mHeader, char* inmsg);

    private:
        NexVariable vaExtrudersA[EXTRUDERS] = {NexVariable(main_page, 14, "vaExtruder1A") 
        #if EXTRUDERS >1
        ,NexVariable(main_page, 15, "vaExtruder2A")
        #endif
        };
	    NexVariable vaExtrudersT[EXTRUDERS] = {NexVariable(main_page, 18, "vaExtruder1T") 
        #if EXTRUDERS >1
        ,NexVariable(main_page, 19, "vaExtruder2T")
        #endif
        };

        NexVariable vaBedA = NexVariable(main_page, 16, "vaBedA");
	    NexVariable vaBedT = NexVariable(main_page, 20, "vaBedT");

        NexVariable vaFan = NexVariable(main_page, 27, "vaFan");
        NexVariable vaIsPrinting = NexVariable(main_page, 12, "vaIsPrinting");
        NexVariable vaLastMessage = NexVariable(main_page, 24, "vaLastMessage");
        NexVariable vaHasFan = NexVariable(main_page, 25, "vaHasFan");
        NexVariable vaExtruders = NexVariable(main_page, 30, "vaExtruders");

        NexText tKillMessage = NexText(10, 3, "tKillMessage");

        NexVariable vaAlert = NexVariable(main_page, 29, "vaAlert");

        NexPicture pSD = NexPicture(main_page, 11, "pSD");
        NexPicture pHome = NexPicture(main_page, 28, "pHome");

	    NexVariable vaX = NexVariable(movement_page, 26, "vaX");
	    NexVariable vaY = NexVariable(movement_page, 27, "vaY");
	    NexVariable vaZ = NexVariable(movement_page, 28, "vaZ");

        NexText tElapsed = NexText(printing_page, 26, "tElapsed");
        NexText tETA = NexText(printing_page, 27, "tETA");
        NexText tMessage = NexText(printing_page, 6, "tMessage");
        NexVariable vaProgress = NexVariable(printing_page, 23, "vaProgress");
        NexVariable vaPPIsPrinting = NexVariable(printing_page, 22, "vaIsPrinting");
        NexVariable vaPPBedA = NexVariable(printing_page, 14, "vaBedA");
        NexVariable vaPPBedT = NexVariable(printing_page, 16, "vaBedT");
        NexVariable vaPPExtrudersA[EXTRUDERS] = {NexVariable(printing_page, 11, "vaExtruder1A") 
        #if EXTRUDERS >1
        ,NexVariable(printing_page, 12, "vaExtruder2A")
        #endif
        };
        NexVariable vaPPExtrudersT[EXTRUDERS] = {NexVariable(printing_page, 14, "vaExtruder1T") 
        #if EXTRUDERS >1
        ,NexVariable(printing_page, 15, "vaExtruder2T")
        #endif
        };
        NexVariable vaPaused = NexVariable(printing_page, 23, "vaPaused");

        NexVariable vaLastPos = NexVariable(SD_page, 11, "vaLastPos");
        NexVariable vaMax = NexVariable(SD_page, 14, "vaMax");
        NexButton list_lines[num_of_lines] = {NexButton(SD_page, 1, "b0"), NexButton(SD_page, 2, "b1"), NexButton(SD_page, 3, "b2"),
                                              NexButton(SD_page, 4, "b3"), NexButton(SD_page, 5, "b4"), NexButton(SD_page, 6, "b5")};

        float _x, _y, _z;
	    int16_t _et[EXTRUDERS], _fan, _bt;
	    uint8_t  _ba, _ea[EXTRUDERS];
	    int  _page = 0;
	    bool _pageChanged = false;
	    //bool _caseLight, _power = 0;
	    bool _isPrinting = false;
	    bool _isHomed = false;
	    bool _startup = false;

        uint8_t _last_perc;

	    int _minutes;

        bool init_status = false;
    };
    #endif
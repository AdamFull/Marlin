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
        void setTPercentage(const char * inStr);

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
        inline void setSDFileCount(uint16_t counted) 
        {
            nof = counted;
            uint16_t max_val = counted <= num_of_lines ? 0 : counted - num_of_lines;
        }
        void update_sd(char files_list[64][27], uint16_t files_less, uint16_t files_count); //SD card scroll list
        uint16_t get_nof() { return nof; }
        const char* get_sd_text(int pos) 
        {
            char* buffer;
            list_lines[pos].getText(buffer, 27);
            return buffer;
        }

        inline void setHasFan(bool state) { vaHasFan.setValue(state); }

        void setIsPrinting(bool status);
        void setProgress(uint8_t perc);
	    void setIsHomed(bool status);
	    bool getIsHomed();

        void setStarted();
	    bool isStarted();

        void set_allert_screen(PGM_P message);

        char* XPortMsg(char* mHeader, char* inmsg);

    private:
        NexVariable vaExtruder1A = NexVariable(main_page, 14, "vaExtruder1A");
	    NexVariable vaExtruder1T = NexVariable(main_page, 18, "vaExtruder1T");
        #if EXTRUDERS > 1
            NexVariable vaExtruder2A = NexVariable(main_page, 15, "vaExtruder2A");
	        NexVariable vaExtruder2T = NexVariable(main_page, 19, "vaExtruder2T");
        #endif

        NexVariable vaBedA = NexVariable(main_page, 16, "vaBedA");
	    NexVariable vaBedT = NexVariable(main_page, 20, "vaBedT");

        NexVariable vaFan = NexVariable(main_page, 27, "vaFan");
        NexVariable vaIsPrinting = NexVariable(main_page, 12, "vaIsPrinting");
        NexVariable vaLastMessage = NexVariable(main_page, 24, "vaLastMessage");
        NexVariable vaHasFan = NexVariable(main_page, 25, "vaHasFan");

        NexVariable vaAlert = NexVariable(main_page, 29, "vaAlert");

        NexButton bSDCard = NexButton(main_page, 5, "main_page.bSDCard");
        NexPicture pSD = NexPicture(main_page, 11, "main_page.pSD");
        NexPicture pHome = NexPicture(main_page, 28, "main_page.pHome");

	    NexVariable vaX = NexVariable(movement_page, 26, "move_page.vaX");
	    NexVariable vaY = NexVariable(movement_page, 27, "move_page.vaY");
	    NexVariable vaZ = NexVariable(movement_page, 28, "move_page.vaZ");

	    //NexText tLayers = NexText(printing_page, tLayers_id, "main_page.tLayers");
	    NexText tETA = NexText(printing_page, 16, "printing_page.tETA");
	    NexText tPercentage = NexText(printing_page, 15, "printing_page.tPercentage");
        NexProgressBar jPrintProgress = NexProgressBar(printing_page, 1, "printing_page.jPrintProgress");

        NexVariable vaLastPos = NexVariable(SD_page, 11, "sd_page.vaLastPos");
        NexVariable vaMax = NexVariable(SD_page, 14, "sd_page.vaMax");
        NexButton list_lines[num_of_lines] = {NexButton(SD_page, 1, "sd_page.b0"), NexButton(SD_page, 2, "sd_page.b1"), NexButton(SD_page, 3, "sd_page.b2"),
                                              NexButton(SD_page, 4, "sd_page.b3"), NexButton(SD_page, 5, "sd_page.b4"), NexButton(SD_page, 6, "sd_page.b5")};

        char _x[7], _y[7], _z[7];
	    char _et[4], _et1[4], _fan[4], _bt[4];
	    uint8_t  _ba, _ea, _ea1;
	    int  _page = 0;
	    bool _pageChanged = false;
	    //bool _caseLight, _power = 0;
	    bool _isPrinting = false;
	    bool _isHomed = false;
	    bool _startup = false;

        char ext[10];
	    int _minutes;

        uint16_t nof;

        bool init_status = false;
    };
    #endif
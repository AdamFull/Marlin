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

        void setTLayers(const char * inStr);
        void setTETA(const char * inStr);
        void setTPercentage(const char * inStr);

        void setPower(bool status);
        void setCaseLight(bool status);

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

        inline void setSDState(bool enabled) { pSD.setPic(enabled ? 22 : 23); }
        inline void setSDFileCount(uint16_t counted) 
        {
            nof = counted;
            uint16_t max_val = counted <= num_of_lines ? 0 : counted - num_of_lines;
            hScroll.setMaxval(max_val);
            hScroll.setValue(max_val);
        }
        void update_sd(char files_list[64][27], uint16_t files_less, uint16_t files_count); //SD card scroll list
        uint16_t get_nof() { return nof; }
        const char* get_sd_text(int pos) 
        {
            char* buffer;
            list_lines[pos].getText(buffer, 27);
            return buffer;
        }

        void setIsPrinting(bool status);
	    void setIsHomed(bool status);
	    bool getIsHomed();

        void setStarted();
	    bool isStarted();

    private:
        NexText tExtruder1A = NexText(main_page, tExtruder1A_id, "tExtruder1A");
	    NexText tExtruder1T = NexText(main_page, tExtruder1T_id, "tExtruder1T");
        #if EXTRUDERS > 1
            NexText tExtruder2A = NexText(main_page, tExtruder2A_id, "tExtruder2A");
	        NexText tExtruder2T = NexText(main_page, tExtruder2T_id, "tExtruder2T");
        #endif

	    NexText tBedT = NexText(main_page, tBedT_id, "tBedT");
	    NexText tBedA = NexText(main_page, tBedA_id, "tBedA");

	    NexText tX = NexText(main_page, tX_id, "tX");
	    NexText tY = NexText(main_page, tY_id, "tY");
	    NexText tZ = NexText(main_page, tZ_id, "tZ");
        
	    NexText tMessage = NexText(main_page, tMessage_id, "main_page.tMessage");

	    NexText tFan = NexText(main_page, tFan_id, "tFan");

	    NexText tLayers = NexText(main_page, tLayers_id, "main_page.tLayers");
	    NexText tETA = NexText(main_page, tETA_id, "main_page.tETA");
	    NexText tPercentage = NexText(main_page, tPercentage_id, "main_page.tPercentage");

        NexDSButton btPower = NexDSButton(menu_page, btPower_id, "menu_page.btPower");
	    NexDSButton btLight = NexDSButton(menu_page, btLight_id, "menu_page.btLight");

	    NexPicture pPower = NexPicture(main_page, pPower_id, "main_page.pPower");
	    NexPicture pLight = NexPicture(main_page, pLight_id, "main_page.pLight");

	    NexVariable vaPower = NexVariable(menu_page, vaPower_id, "menu_page.vaPower");
	    NexVariable vaLight = NexVariable(menu_page, vaLight_id, "menu_page.vaLight");

	    NexPicture pHome = NexPicture(main_page, pHome_id, "main_page.pHome");
	    NexPicture pExtruding = NexPicture(main_page, pExtruding_id, "main_page.pExtruding");

	    NexTimer tmSS = NexTimer(main_page, tmSS_id, "tmSS");
        
	    NexVariable vaCounter = NexVariable(main_page, vaCounter_id, "vaCounter");

        NexButton bSDCard = NexButton(main_page, 8, "main_page.bSDCard");
        NexPicture pSD = NexPicture(main_page, 25, "main_page.pSD");
        NexSlider hScroll = NexSlider(SD_page, 2, "sd_page.hScroll");
        NexButton list_lines[num_of_lines] = {NexButton(SD_page, 1, "sd_page.b0"), NexButton(SD_page, 2, "sd_page.b1"), NexButton(SD_page, 3, "sd_page.b2"),
                                              NexButton(SD_page, 4, "sd_page.b3"), NexButton(SD_page, 5, "sd_page.b4"), NexButton(SD_page, 6, "sd_page.b5")};

        char _x[7], _y[7], _z[7];
	    char _et[4], _fan[4], _bt[4];
	    uint8_t  _ba, _ea;
	    int  _page = 0;
	    bool _pageChanged = false;
	    bool _caseLight, _power = 0;
	    bool _isPrinting = false;
	    bool _isHomed = false;
	    bool _startup = false;

        char ext[10];
	    int _minutes;

        uint16_t nof;

        bool init_status = false;
    };
    #endif
#ifndef NEXTIONLCDELEMENTS_H
#define NECTIONLCDELEMENTS_H

#include "MarlinConfig.h"

#if ENABLED(NEXTION_LCD)

    #include "Nextion.h"

    #if ENABLED(NEXTION_TIME)
        #include "Wire.h"
        #include "TimeLib.h"
    #endif

    #define LOGO_PAGE 0
    #define LOADING_B 1
    #define LOADING_E 4
    #define MAIN_PAGE 5
    #define MENU_PAGE 6
    #define HEAT_B 7
    #define HEAT_E 10
    #define FAN_B 11
    #define FAN_E 13
    #define EXTRUDER_PAGE 14
    #define MOVEMENT_PAGE 21
    #define GCODE_PAGE
    #define SAVER_PAGE

    class nextionlcdelements    //Nextion visual components control
    {
    public:
        void setExtruderTemperature(int targetTemp, int extruderId);
        void setExtruderActual();
        inline NexText& gettExtruder1A() { return tExtruder1A; }
        void setExtruderTarget();
        inline NexText& gettExtruder1T() { return tExtruder1T; }

        void setBedTemperature(int targetTemp);
        void setBedTarget(char temp[10]);
        inline NexText& gettBedT() { return tBedT; }
        void setBedActual();
        inline NexText& gettBedA() { return tBedA; }

        inline void enableBed() { }

        void setXPos();
        inline NexText& gettX() { return tX; }
        void setYPos();
        inline NexText& gettY() { return tY; }
        void setZPos();
        inline NexText& gettZ() { return tZ; }

        void setMessage(const char * inStr);
        inline NexText& gettMessage() { return tMessage; }

        void setFan(char temp[10]);
        inline NexText& gettFan() { return tFan; }

        void setTLayers(const char * inStr);
        inline NexText& gettLayers() { return tLayers; }
        void setTETA(const char * inStr);
        inline NexText& gettETA() { return tETA; }
        void setTPercentage(const char * inStr);
        inline NexText& gettPercentage() { return tPercentage; }

        inline NexDSButton& getbtPower() { return btPower; }
        inline NexDSButton& getbtLight() { return btLight; }

        inline NexPicture& getpPower() { return pPower; }
        inline NexPicture& getpLight() { return pLight; }

        void setPower(bool status);
        inline NexVariable& getvaPower() { return vaPower; }
        void setCaseLight(bool status);
        inline NexVariable& getvaLight() { return vaLight; }

        inline NexPicture& getpHome() { return pHome; }
        inline NexPicture& getpExtruding() { return pExtruding; }

        inline NexTimer& gettmSS() { return tmSS; }

        inline NexVariable& getvaCounter() { return vaCounter; }

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

        void setIsPrinting(bool status);
	    void setIsHomed(bool status);
	    bool getIsHomed();

        void setStarted();
	    bool isStarted();

    private:
        NexText tExtruder1A = NexText(1, 28, "tExtruder1A");
	    NexText tExtruder1T = NexText(1, 27, "tExtruder1T");

	    NexText tBedT = NexText(1, 6, "tBedT");
	    NexText tBedA = NexText(1, 5, "tBedA");

	    NexText tX = NexText(1, 16, "tX");
	    NexText tY = NexText(1, 17, "tY");
	    NexText tZ = NexText(1, 18, "tZ");

	    NexText tMessage = NexText(1, 7, "MainPage.tMessage");

	    NexText tFan = NexText(1, 21, "tFan");

	    NexText tLayers = NexText(1, 21, "MainPage.tLayers");
	    NexText tETA = NexText(1, 19, "MainPage.tETA");
	    NexText tPercentage = NexText(1, 8, "MainPage.tPercentage");

        NexDSButton btPower = NexDSButton(2, 5, "MainMenu.btPower");
	    NexDSButton btLight = NexDSButton(2, 6, "MainMenu.btLight");

	    NexPicture pPower = NexPicture(1, 29, "MainPage.pPower");
	    NexPicture pLight = NexPicture(1, 30, "MainPage.pLight");

	    NexVariable vaPower = NexVariable(2, 8, "MainMenu.vaPower");
	    NexVariable vaLight = NexVariable(2, 9, "MainMenu.vaLight");

	    NexPicture pHome = NexPicture(1, 35, "MainPage.pHome");
	    NexPicture pExtruding = NexPicture(1, 34, "MainPage.pExtruding");

	    NexTimer tmSS = NexTimer(1, 37, "tmSS");
        
	    NexVariable vaCounter = NexVariable(1, 38, "vaCounter");

        char _x[7], _y[7], _z[7];
	    char _et[4], _fan[4], _bt[4];
	    uint8_t  _ba, _ea;
	    int  _page = 0;
	    bool _pageChanged;
	    bool _caseLight, _power = 0;
	    bool _isPrinting = false;
	    bool _isHomed = false;
	    bool _startup = false;

        char ext[10];
	    int _minutes;

        bool init_status = false;
    };
    #endif

#endif
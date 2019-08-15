#include "nextionlcdelements.h"
#include "language.h"

#if ENABLED(NEXTION_LCD)

	void nextionlcdelements::setXPos(char * inStr)
	{
		if (strcmp(inStr, _x)!=0 || _pageChanged || !isStarted())
		{
			this->tX.setText(inStr);
			strcpy(_x, inStr);
		}
	}

	void nextionlcdelements::setYPos(char * inStr) 
	{
		if (strcmp(inStr, _y) != 0 || _pageChanged || !isStarted())
		{
			this->tY.setText(inStr);
			strcpy(_y, inStr);
		}
	}

	void nextionlcdelements::setZPos(char * inStr)
	{
		if (strcmp(inStr, _z) != 0 || _pageChanged || !isStarted())
		{
			this->tZ.setText(inStr);
			strcpy(_z, inStr);
		}
	}

	void nextionlcdelements::setBedTarget(char * inTemp)
	{
		if (strcmp(inTemp, _bt) != 0 || _pageChanged || !isStarted())
		{
			this->tBedT.setText(inTemp);
			strcpy(_bt, inTemp);
		}
	}

	void nextionlcdelements::setBedActual(uint8_t inTemp)
	{
		uint16_t average = (uint8_t)((_ba + inTemp) / 2);
		if (_ba != average || _pageChanged || !isStarted())
		{
			this->tBedA.setText(itoa(average, ext, 10));
			_ba = average;
		}
	}

	void nextionlcdelements::setExtruderTarget(char * inTemp, int inExtruder)
	{
		if (strcmp(inTemp, _et) != 0 || _pageChanged || !isStarted())
		{
			this->tExtruder1T.setText(inTemp);
			strcpy(_et, inTemp);
		}
	}

	void nextionlcdelements::setExtruderActual(uint8_t inTemp, int inExtruder)
	{

		uint16_t average = (uint8_t)((_ea + inTemp) / 2);
		if (_ea != average || _pageChanged || !isStarted())
		{
			this->tExtruder1A.setText(itoa(average, ext, 10));
			_ea = average;
		}
	}

	void nextionlcdelements::setFan(char * inSpeed)
	{
		if (strcmp(inSpeed, _fan) != 0 || _pageChanged || !isStarted())
		{
			this->tFan.setText(inSpeed);
			strcpy(_fan, inSpeed);
		}
	}

	void nextionlcdelements::setPage(int page)
	{
		if (page != _page)
			_pageChanged = true;
		_page = page;
	}

	int nextionlcdelements::getPage()
	{
		return _page;
	}

	void nextionlcdelements::resetPageChanged()
	{
		_pageChanged = false;
	}

	bool nextionlcdelements::pageChanged()
	{
		return _pageChanged;
	}

	void nextionlcdelements::setPower(bool status)
	{
		if (_power != status || _pageChanged || !isStarted())
		{
			this->_power = status;

			this->vaPower.setValue(_power ? 1 : 0);
			if (_page == 2)
				this->btPower.setValue(_power ? 1 : 0);
			else if (_page == 1)
            	status ? this->pPower.setPic(19) : this->pPower.setPic(20);
		}
	}

	void nextionlcdelements::setCaseLight(bool status)
	{
		if (_caseLight != status || _pageChanged || !isStarted())
		{
			this->_caseLight = status;

			this->vaLight.setValue(_caseLight ? 1 : 0);
			if (_page == 2)
				this->btLight.setValue(_caseLight ? 1 : 0);
			else if (_page == 1)
			{
				//uint32_t val = atoi(subbuff);
            	status == 1 ? this->pLight.setPic(18) : this->pLight.setPic(20);
			}
		}
	}

	void nextionlcdelements::setIsPrinting(bool status)
	{
		if (_isPrinting != status || _pageChanged || !isStarted())
		{
			_isPrinting = status;

			if (status == 1)
			{
				this->pExtruding.setPic(23);
				this->tmSS.disable();
				this->vaCounter.setValue(0);
			}
			else
			{
				this->pExtruding.setPic(20);
				this->tmSS.enable();
				this->vaCounter.setValue(0);
			}
		}
	}

	void nextionlcdelements::setIsHomed(bool status)
	{
		if (_isHomed != status || _pageChanged || !isStarted())
		{
			_isHomed = status;
        	status == 1 ? this->pHome.setPic(24) : this->pHome.setPic(20);
		}
	}

	bool nextionlcdelements::getIsHomed()
	{
		return _isHomed;
	}

	void nextionlcdelements::setStarted()
	{
		_startup = true;
	}

	bool nextionlcdelements::isStarted()
	{
		return _startup;
	}


	void nextionlcdelements::setMessage(const char * inStr)
	{
		this->tMessage.setText(inStr);
	}

	void nextionlcdelements::setTLayers(const char * inStr)
	{
		this->tLayers.setText(inStr);
	}

	void nextionlcdelements::setTETA(const char * inStr)
	{
		this->tETA.setText(inStr);
	}

	void nextionlcdelements::setTPercentage(const char * inStr)
	{
		this->tPercentage.setText(inStr);
	}

	#if ENABLED(NEXTION_TIME)

	//void PrinterClass::setTime(int year, int month, int day, int hour, int minute)
	void nextionlcdelements::setTime()
	{
		int myMinute = minute();
		if (myMinute != _minutes)
		{
			_minutes = myMinute;

			int myMonth = month();
			int myDay = day();
			int myHour = hour();


			char myBuff[5];

			strcpy(ext, itoa(year(), myBuff, 10));
			strcat(ext, "-");

			if (myMonth < 10)
				strcat(ext, "0");
			strcat(ext, itoa(myMonth, myBuff, 10));
		
		
			strcat(ext, "-");

			if (myDay < 10)
				strcat(ext, "0");
			strcat(ext, itoa(day(), myBuff, 10));


			strcat(ext, " ");

			if (myHour < 10)
				strcat(ext, "0");
			strcat(ext, itoa(hour(), myBuff, 10));

			strcat(ext, ":");

			if (myMinute < 10)
				strcat(ext, "0");
			strcat(ext, itoa(myMinute, myBuff, 10));

			tTime.setText(ext);
		}
	}
	#endif
#endif
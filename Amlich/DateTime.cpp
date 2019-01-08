//#pragma once

//#pragma once
#include "DateTime.h"

//DateTime Function
unsigned char DateTime::getHour()
{
	return time.hour;
}

unsigned char DateTime::getMinute()
{
	return time.minute;
}

unsigned char DateTime::getSecond()
{
	return time.second;
}

unsigned char DateTime::getDay()
{
	return date.day;
}

unsigned char DateTime::getDate()
{
	return date.date;
}

unsigned char DateTime::getMonth()
{
	return date.month;
}

unsigned char DateTime::getYear()
{
	return date.year;
}

unsigned char DateTime::getMaxDate()
{
	return date.getMaxDate();
}

bool DateTime::isLeafYear()
{
	return false;
}

bool DateTime::isLeafMonth()
{
	return lunarDate.isLeafMonth;
}

unsigned char DateTime::getLunarDate()
{
	return lunarDate.date;
}

unsigned char DateTime::getLunarMonth()
{
	return lunarDate.month;
}

void DateTime::setLunarDate()
{
	Date tempDt = date.getLunarDateMonth();
	lunarDate.date = tempDt.date;
	lunarDate.month = tempDt.month;
	lunarDate.year = tempDt.year;
	lunarDate.isLeafMonth = date.isLeafMonth;
}

void DateTime::setTime(unsigned char HH, unsigned char mm)
{
	time.hour = HH;
	time.minute = mm;
	time.second = 00;
}

void DateTime::setDate(unsigned char d, unsigned char dd, unsigned char MM, unsigned char yy)
{
	date.day = d;
	date.date = dd;
	date.month = MM;
	date.year = yy;
	setLunarDate();
}

TimeSpan::TimeSpan()
{
	hour = 16;
	minute = 23;
	second = 0;
}
//Date Function
Date::Date()
{
	day = 1;
	date = 24;
	month = 9;
	year = 18;
}

Date::Date(unsigned char _date, unsigned char _month, unsigned char _year)
{
	day = 0;
	date = _date;
	month = _month;
	year = _year;
}
bool Date::isLeafYear()
{
	int realyear = 2000 + year;
	return (realyear % 4 == 0) && ((realyear % 400 == 0) || (realyear % 100 != 0));
}

unsigned char Date::getMaxDate()
{
	char number;
	switch (month)
	{
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
	{
		number = 31;
		break;
	}
	case 2:
	{
		if (isLeafYear())
		{
			number = 29;
		}
		else
		{
			number = 28;
		}
		break;
	}
	default:
	{
		number = 30;
		break;
	}
	}
	return number;
}

bool Date::operator>(const Date d2)
{
	return (year > d2.year) || (year == d2.year && month > d2.month) || (year == d2.year && month == d2.month && date > d2.date);
}

bool Date::operator<(const Date d2)
{
	return (year < d2.year) || (year == d2.year && month < d2.month) || (year == d2.year && month == d2.month && date < d2.date);
}

bool Date::operator=(const Date d2)
{
	return (year == d2.year && month == d2.month && date == d2.date);
}

Date Date::operator+(const unsigned char b)
{
	Date addedDate(date + b, month, year);
	while (addedDate.date > addedDate.getMaxDate())
	{
		addedDate.date = addedDate.date - addedDate.getMaxDate();
		addedDate.month++;
		if (addedDate.month == 13)
		{
			addedDate.year++;
			addedDate.month = 1;
		}
	}
	return addedDate;
}

unsigned char GetMaxDay(unsigned char lunarMonth, unsigned char byte1, unsigned char byte2, unsigned char byte3)
{
	unsigned char GetValue = 0;
	if (lunarMonth == 0)
		return 0;
	else if (lunarMonth == 1)
		GetValue = byte3 & 0x01;
	else if (lunarMonth == 2)
		GetValue = byte3 & 0x02;
	else if (lunarMonth == 3)
		GetValue = byte3 & 0x04;
	else if (lunarMonth == 4)
		GetValue = byte3 & 0x08;
	else if (lunarMonth == 5)
		GetValue = byte3 & 0x10;
	else if (lunarMonth == 6)
		GetValue = byte3 & 0x20;
	else if (lunarMonth == 7)
		GetValue = byte3 & 0x40;
	else if (lunarMonth == 8)
		GetValue = byte3 & 0x80;
	else if (lunarMonth == 9)
		GetValue = byte2 & 0x01;
	else if (lunarMonth == 10)
		GetValue = byte2 & 0x02;
	else if (lunarMonth == 11)
		GetValue = byte2 & 0x04;
	else if (lunarMonth == 12)
		GetValue = byte2 & 0x08;
	else if (lunarMonth == 13)
		GetValue = byte1 & 0x02; //leap month
	if (GetValue != 0x00)
		return 30;
	return 29;
}

Date Date::getLunarDateMonth()
{
	unsigned char sD, sM, lD, lM, lY, aM;
	/*
	sD: start day
	sM: start Month
	lD: lunar Date
	lM: Lunar Month
	lY: lunar year
	aM: number days in each lunar month
	*/
	unsigned char roottime = 16;
	bool CNN = true;
	//Date Start;
	unsigned char byte1, byte2, byte3;
	if (year < roottime)
		return Date(date, month, year);
	// Get Lut 3 bytes
	byte1 = LUT2K16[(year - roottime) * 3];
	byte2 = LUT2K16[(year - roottime) * 3 + 1];
	byte3 = LUT2K16[(year - roottime) * 3 + 2];
	lY = year;
	//Get Start Day Start Month
	sD = byte1 >> 3;
	if ((byte1 & 0x04) == 0)
		sM = 1;
	else
		sM = 2;
	Date cmpDate(sD, sM, year);
	Date currentDate(date, month, year);
	//Get LUT for previous year if this Start Day < Current Day
	if (currentDate < cmpDate)
	{
		if (year < 17)
			return Date(date, month, year);
		byte1 = LUT2K16[(year - roottime) * 3 - 3];
		byte2 = LUT2K16[(year - roottime) * 3 - 2];
		byte3 = LUT2K16[(year - roottime) * 3 - 1];
		lY = year - 1;
		sD = byte1 >> 3;
		if ((byte1 & 0x04) == 0)
			sM = 1;
		else
			sM = 2;
	}
	CNN = (byte2 >> 4 != 0); // Check if nular month set
							 //cmpDate =
	cmpDate.date = sD;
	cmpDate.month = sM;
	cmpDate.year = lY;
	lM = 1;
	aM = GetMaxDay(lM, byte1, byte2, byte3);
	isLeafMonth = false;
	for (unsigned char i = 0; i < 13; i++)
	{
		// get max day of lunar month
		//aM = GetMaxDay(lM, byte1, byte2, byte3);
		Date TempDate = cmpDate + aM;
		if (TempDate > currentDate)
		{
			break;
		}
		else
		{
			if (lM == (byte2 >> 4) && CNN)
			{
				CNN = false;
				aM = GetMaxDay(13, byte1, byte2, byte3);
				isLeafMonth = true;
			}
			else
			{
				lM++;
				aM = GetMaxDay(lM, byte1, byte2, byte3);
			}
			cmpDate.date = TempDate.date;
			cmpDate.month = TempDate.month;
			cmpDate.year = TempDate.year;
		}
	}
	if (cmpDate.month == month)
	{
		lD = date - cmpDate.date + 1;
	}
	else
	{
		lD = date + cmpDate.getMaxDate() - cmpDate.date + 1;
	}
	return Date(lD, lM, year);
}

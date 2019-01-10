//#pragma once

//#pragma once
#include "DateTime.h"

//DateTime Function
uint8_t DateTime::getHour()
{
	return _time._hour;
}

uint8_t DateTime::getMinute()
{
	return _time._minute;
}

uint8_t DateTime::getSecond()
{
	return _time._second;
}

uint8_t DateTime::getDayOfWeek()
{
	return _date._day;
}

uint8_t DateTime::getDate()
{
	return _date._date;
}

uint8_t DateTime::getMonth()
{
	return _date._month;
}

uint8_t DateTime::getYear()
{
	return _date._year;
}

uint8_t DateTime::getMaxDate()
{
	return _date.getMaxDate();
}

bool DateTime::isLeafYear()
{
	return false;
}

bool DateTime::isLeafMonth()
{
	return _lunarDate._isLeafMonth;
}

uint8_t DateTime::getLunarDate()
{
	return _lunarDate._date;
}

uint8_t DateTime::getLunarMonth()
{
	return _lunarDate._month;
}

DateTime::DateTime(Date date, TimeSpan time)
{
	_date = date;
	_time = time;
	_lunarDate = _date.getLunarDateMonth();
}

TimeSpan::TimeSpan()
{
	_hour = 16;
	_minute = 23;
	_second = 0;
}

TimeSpan::TimeSpan(uint8_t h, uint8_t m, uint8_t s){
	_hour = h;
	_minute = m;
	_second = s;
}

//Date Function
Date::Date()
{
	_day = 1;
	_date = 24;
	_month = 9;
	_year = 18;
}

Date::Date(uint8_t date, uint8_t month, uint8_t year)
{
	_date = date;
	_month = month;
	_year = year;
	_day = getDayOfWeek();
	_isLeafYear = isLeafYear();
	_isLeafMonth = false;
}

uint8_t Date::getDayOfWeek()	
{
	uint16_t y = _year + 2000;
	static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
	y -= _month < 3;
	return (y + y/4 - y/100 + y/400 + t[_month-1] + _date) % 7;
}

bool Date::isLeafYear()
{
	int realyear = 2000 + _year;
	return (realyear % 4 == 0) && ((realyear % 400 == 0) || (realyear % 100 != 0));
}

uint8_t Date::getMaxDate()
{
	char number;
	switch (_month)
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
	return (_year > d2._year) || 
		(_year == d2._year && _month > d2._month) ||
		(_year == d2._year && _month == d2._month && _date > d2._date);
}

bool Date::operator<(const Date d2)
{
	return (_year < d2._year) || 
	(_year == d2._year && _month < d2._month) ||
	(_year == d2._year && _month == d2._month && _date < d2._date);
}

bool Date::operator=(const Date d2)
{
	return (_year == d2._year && _month == d2._month && _date == d2._date);
}

Date Date::operator+(const uint8_t b)
{
	Date addedDate(_date + b, _month, _year);
	while (addedDate._date > addedDate.getMaxDate())
	{
		addedDate._date = addedDate._date - addedDate.getMaxDate();
		addedDate._month++;
		if (addedDate._month == 13)
		{
			addedDate._year++;
			addedDate._month = 1;
		}
	}
	return addedDate;
}

uint8_t GetMaxDay(uint8_t lunarMonth, uint8_t byte1, uint8_t byte2, uint8_t byte3)
{
	uint8_t GetValue = 0;
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
	uint8_t sD, sM, lD, lM, lY, aM;
	/*
	sD: start day
	sM: start Month
	lD: lunar Date
	lM: Lunar Month
	lY: lunar year
	aM: number days in each lunar month
	*/
	uint8_t roottime = 16;
	bool CNN = true;
	//Date Start;
	uint8_t byte1, byte2, byte3;
	if (_year < roottime)
		return Date(_date, _month, _year);
	// Get Lut 3 bytes
	byte1 = LUT2K16[(_year - roottime) * 3];
	byte2 = LUT2K16[(_year - roottime) * 3 + 1];
	byte3 = LUT2K16[(_year - roottime) * 3 + 2];
	lY = _year;
	//Get Start Day Start Month
	sD = byte1 >> 3;
	if ((byte1 & 0x04) == 0)
		sM = 1;
	else
		sM = 2;
	Date cmpDate(sD, sM, _year);
	Date currentDate(_date, _month, _year);
	//Get LUT for previous year if this Start Day < Current Day
	if (currentDate < cmpDate)
	{
		if (_year < 17)
			return Date(_date, _month, _year);
		byte1 = LUT2K16[(_year - roottime) * 3 - 3];
		byte2 = LUT2K16[(_year - roottime) * 3 - 2];
		byte3 = LUT2K16[(_year - roottime) * 3 - 1];
		lY = _year - 1;
		sD = byte1 >> 3;
		if ((byte1 & 0x04) == 0)
			sM = 1;
		else
			sM = 2;
	}
	CNN = (byte2 >> 4 != 0); 
	cmpDate._date = sD;
	cmpDate._month = sM;
	cmpDate._year = lY;
	lM = 1;
	aM = GetMaxDay(lM, byte1, byte2, byte3);
	_isLeafMonth = false;
	for (uint8_t i = 0; i < 13; i++)
	{
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
				_isLeafMonth = true;
			}
			else
			{
				lM++;
				aM = GetMaxDay(lM, byte1, byte2, byte3);
			}
			cmpDate._date = TempDate._date;
			cmpDate._month = TempDate._month;
			cmpDate._year = TempDate._year;
		}
	}
	if (cmpDate._month == _month)
	{
		lD = _date - cmpDate._date + 1;
	}
	else
	{
		lD = _date + cmpDate.getMaxDate() - cmpDate._date + 1;
	}
	return Date(lD, lM, _year);
}

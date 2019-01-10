

/*
Luu am lich 3byte/nam
byte1:    [Ngay bat dau 1/1 AL - 5 bit][thang bat dau 1/1 AL - 1 bit][So ngay thang nhuan - 1 bit]
byte2:    [Thang nhuan - 4 bit][so ngay thang 12,11,10,9 - 4bit]
byte3:    [So ngay thang 8,7,6,5,4,3,2,1 - 8bit]

Ngay bat dau 1/1 AL  :  0x01->0x1F    1-31
thang bat dau 1/1 AL :  0             Thang 1
1             Thang 2
So ngay trong thang  :  0             29 ngay
1             30 ngay


*/

#ifndef DateTime_h
#define DateTime_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#endif

const uint8_t LUT2K16[] = {68, 13, 165,   // 2k16
								 226, 110, 138, // 2k17
								 132, 14, 74,   // 2k18
								 44, 12, 150,   // 2k19
								 200, 12, 150,  // 2k20
								 100, 5, 86,	// 2k21
								 12, 10, 181,   // 2k22
								 176, 42, 218,  // 2k23
								 84, 6, 210,	// 2k24
								 232, 103, 101, // 2k25
								 140, 7, 37,	// 2k26
								 52, 6, 75,		// 2k27
								 208, 86, 87,   // 2k28
								 104, 0, 0};	// 2k29
const uint8_t LEDCode[] = {192, 249, 164, 176, 153, 146, 130, 248, 128, 144};
//0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90
class TimeSpan
{
  public:
	uint8_t _hour;
	uint8_t _minute;
	uint8_t _second;
	TimeSpan();
	TimeSpan(uint8_t h, uint8_t m, uint8_t s);
};

class Date
{
  public:
	uint8_t _day;
	uint8_t _date;
	uint8_t _month;
	uint8_t _year;
	bool _isLeafMonth;
	bool _isLeafYear;
	Date();
	Date(uint8_t date, uint8_t month, uint8_t year);
	uint8_t getMaxDate();
	bool operator>(const Date d2);
	bool operator<(const Date d2);
	bool operator=(const Date d2);
	Date operator+(uint8_t b);
	Date getLunarDateMonth();
	uint8_t getDayOfWeek();
	bool isLeafYear();
};

class DateTime
{
  public:
	uint8_t getHour();
	uint8_t getMinute();
	uint8_t getSecond();
	uint8_t getDate();
	uint8_t getDayOfWeek();
	uint8_t getMonth();
	uint8_t getYear();
	uint8_t getMaxDate();
	bool isLeafYear();
	bool isLeafMonth();
	uint8_t getLunarDate();
	uint8_t getLunarMonth();
	DateTime(Date date, TimeSpan time);
	
private :	
	TimeSpan _time;
	Date _date;
	Date _lunarDate;
};

#endif

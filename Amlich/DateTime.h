

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

const unsigned char LUT2K16[] = {68, 13, 165,   // 2k16
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
const unsigned char LEDCode[] = {192, 249, 164, 176, 153, 146, 130, 248, 128, 144};
//0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90
class TimeSpan
{
  public:
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	TimeSpan();
};

class Date
{
  public:
	unsigned char day;
	unsigned char date;
	unsigned char month;
	unsigned char year;
	bool isLeafMonth;
	bool isLeafYear();
	Date();
	Date(unsigned char _date, unsigned char _month, unsigned char _year);
	unsigned char getMaxDate();
	bool operator>(const Date d2);
	bool operator<(const Date d2);
	bool operator=(const Date d2);
	Date operator+(const unsigned char b);
	Date getLunarDateMonth();
};

class DateTime
{
  public:
	TimeSpan time;
	Date date;
	Date lunarDate;
	unsigned char getHour();
	unsigned char getMinute();
	unsigned char getSecond();
	unsigned char getDate();
	unsigned char getDay();
	unsigned char getMonth();
	unsigned char getYear();
	unsigned char getMaxDate();
	bool isLeafYear();
	bool isLeafMonth();
	unsigned char getLunarDate();
	unsigned char getLunarMonth();
	void setTime(unsigned char HH, unsigned char mm);
	void setDate(unsigned char d, unsigned char dd, unsigned char MM, unsigned char yy);
	void setLunarDate();
	void shiftData(int dataPin, int clockPin, int slatchPin);
};

#endif

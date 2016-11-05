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


#ifndef Amlich_h
#define Amlich_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

class NgayThangNam{
	public:
	uint8_t ngay;
	uint8_t thang;
	uint8_t nam;
	NgayThangNam();
	NgayThangNam(uint8_t Day, uint8_t Month, uint8_t Year);
	NgayThangNam operator+(const uint8_t& b);
	uint8_t operator-(const NgayThangNam& Day2);
	uint8_t GetMaxDayofMonth();
	bool IsLeapYear();
	NgayThangNam ConvertDay();
	uint8_t Compare2Day(NgayThangNam day2);
};
class LUT{
	public:
	uint8_t byte1;
	uint8_t byte2;
	uint8_t byte3;
	LUT();
	uint8_t LeapMonth();
	uint8_t GetMaxDay(uint8_t month);
};
#endif 














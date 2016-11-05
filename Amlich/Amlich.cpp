/*

	byte1:    [Ngay bat dau 1/1 AL - 5 bit][thang bat dau 1/1 AL - 1 bit][So ngay thang nhuan - 1 bit][Not use - 1 bit]
	byte2:    [Thang nhuan - 4 bit][so ngay thang 12,11,10,9 - 4bit]
	byte3:    [So ngay thang 8,7,6,5,4,3,2,1 - 8bit]
	
	class NgayThangNam{
		uint8_t ngay;
		uint8_t thang;
		uint8_t nam;
		NgayThangNam();
		bool IsLeapYear();
	}

	class LUT{
		char byte1;
		char byte2;
		char byte3;
	    uint8_t LeapMonth;
	}
    Amlich();
	NgayThangNam        ConvertDay(NgayThangNam current, LUT year);
	uint8_t             (NgayThangNam) day1.Compare2Day( NgayThangNam Day2);
	                    2 if day1 > day 2
						1 if day1 == day 2
						0 if day1 < day 2
	uint8_t             Minus(NgayThangNam day1, NgayThangNam day2);
	lut is updated to 2K19
*/

#include "Amlich.h" 
const uint8_t LUT2K10[] = { 68, 11, 165, 226, 110, 138, 132, 14, 74, 44, 12, 150, 200, 0, 0  };
const uint8_t roottime = 16;
NgayThangNam::NgayThangNam(){
	this->ngay = 1;
	this->thang = 1;
    this->nam = 10;	
}

NgayThangNam NgayThangNam:: operator+(const uint8_t& DayNumber){
	NgayThangNam Daysum;
	uint8_t MaxDay = this->GetMaxDayofMonth();
	Daysum.ngay =  this->ngay + DayNumber;	
	Daysum.thang = this->thang;
	Daysum.nam = this->nam;
	if (Daysum.ngay > MaxDay) {
		Daysum.ngay -= MaxDay;
		Daysum.thang ++;
		if (Daysum.thang > 12){
			Daysum.thang = 1;
			Daysum.nam++;
		}
	}
	return Daysum;
}

bool NgayThangNam::IsLeapYear(){
	if (this->nam %4 == 0) return true;
	return false;
}

NgayThangNam::NgayThangNam(uint8_t Day, uint8_t Month, uint8_t Year){
	this->ngay = Day;
	this->thang = Month;
    this->nam = Year;	
}

uint8_t LUT::GetMaxDay(uint8_t month){
	uint8_t GetValue;
	if (month == 1) GetValue = this->byte3 & 0x01;
	if (month == 2) GetValue = this->byte3 & 0x02;
	if (month == 3) GetValue = this->byte3 & 0x04;
	if (month == 4) GetValue = this->byte3 & 0x08;
	if (month == 5) GetValue = this->byte3 & 0x10;
	if (month == 6) GetValue = this->byte3 & 0x20;
	if (month == 7) GetValue = this->byte3 & 0x40;
	if (month == 8) GetValue = this->byte3 & 0x80;
	if (month == 9) GetValue = this->byte2 & 0x01;
	if (month == 10) GetValue = this->byte2 & 0x02;
	if (month == 11) GetValue = this->byte2 & 0x04;
	if (month == 12) GetValue = this->byte2 & 0x08;
	if (month == 13) GetValue = this->byte1 & 0x02;
	if (GetValue != 0x00) return 30;
	return 29;
}

LUT::LUT(){
	this->byte1 = 0x0D;
	this->byte2 = 0x00;
	this->byte3 = 0x00;
}

uint8_t LUT::LeapMonth(){
	return this->byte2 >> 4;
}

NgayThangNam NgayThangNam::ConvertDay(){
	uint8_t sD, sM, lD, lM, lY, D, M, Y, cD, cM, aM;
	NgayThangNam current;
	current.ngay = this->ngay;
	current.thang = this->thang;
	current.nam = this->nam;
	D = current.ngay;
	M = current.thang;
	Y = current.nam;
	bool cNN = true;
	if (Y < 10) return current; // count from 2k10
	LUT lut;
	NgayThangNam start;
	lut.byte1 = LUT2K10[(Y - roottime) * 3];
	lut.byte2 = LUT2K10[(Y - roottime) * 3 + 1];
	lut.byte3 = LUT2K10[(Y - roottime) * 3 + 2];
	D = current.ngay;
	M = current.thang;
	Y = current.nam;
	lY = Y;
	sD = lut.byte1 >> 3;
	if ((lut.byte1 & 0x04) == 0) sM = 1;
	else sM = 2;
	if (current.Compare2Day(NgayThangNam(sD, sM, current.nam)) == 0){
		if (Y < 11) return current;
		lut.byte1 = LUT2K10[(Y - roottime - 1) * 3];
		lut.byte2 = LUT2K10[(Y - roottime - 1) * 3 + 1];
		lut.byte3 = LUT2K10[(Y - roottime - 1) * 3 + 2];
		lY = Y - 1;
	}
	sD = lut.byte1 >> 3;
	if ((lut.byte1 & 0x04) == 0) sM = 1;
	else sM = 2;
	NgayThangNam CountDay = NgayThangNam(sD, sM, lY);
	lM = 1;
	if (lut.LeapMonth() == 0, cNN = false);
	for (int i = 1; i < 13; i++){
		aM = lut.GetMaxDay(lM);
		NgayThangNam Nextday = CountDay + aM;
		if ( Nextday.Compare2Day(current) > 0){
			int j = CountDay + aM - current;
			int k = aM - j;
			CountDay = CountDay + k;
			lD = k + 1;
			if (lD > aM) {
				lD = lD - aM;
				if (!cNN) lM++;
				else cNN = false;
			}
			break;
		}
		else{
			CountDay = CountDay + aM;
			if ((lM == lut.LeapMonth())& cNN){
				cNN = false;
				NgayThangNam Nextday = CountDay + lut.GetMaxDay(13);
				if (Nextday.Compare2Day(current) > 0){
					int j = CountDay + aM - current;
					int k = aM - j;
					CountDay = CountDay + k;
					lD = k + 1;
					if (lD > aM) {
						lD = lD - aM;
					}
					break;
				}
				else{
					CountDay = CountDay + lut.GetMaxDay(13);
					lM++;
				}
			}
			else{
				lM++;
			}
		}
	}
	return NgayThangNam(lD, lM, lY);
}

uint8_t NgayThangNam::Compare2Day( NgayThangNam day2){
	NgayThangNam day1;
	day1.ngay = this->ngay;
	day1.thang = this->thang;
	day1.nam = this->nam;
	if (day1.nam > day2.nam) return 2;
	if (day1.nam < day2.nam) return 0;
	if (day1.thang > day2.thang) return 2;
	if (day1.thang < day2.thang) return 0;
	if (day1.ngay > day2.ngay) return 2;
	if (day1.ngay < day2.ngay) return 0;
	return 1;
} 

uint8_t NgayThangNam::GetMaxDayofMonth(){
	if (this->thang == 1) return 31;
	if (this->thang == 2 )
		if (this->IsLeapYear()) return 29;
	    else return 28;
	if (this->thang == 3) return 31;
	if (this->thang == 4) return 30;
	if (this->thang == 5) return 31;
	if (this->thang == 6) return 30;
	if (this->thang == 7) return 31;
	if (this->thang == 8) return 31;
	if (this->thang == 9) return 30;
	if (this->thang == 10) return 31;
	if (this->thang == 11) return 30;
	if (this->thang == 12) return 31;
	return 0;
}

uint8_t NgayThangNam:: operator-(const NgayThangNam& Day2){
	uint8_t i = 0;
	NgayThangNam Day1 = Day2;
	NgayThangNam Compare;
	Compare.ngay = this->ngay;
	Compare.thang = this->thang;
	Compare.nam = this->nam;
	while (Day1.Compare2Day(Compare) == 0){
		Day1 = Day1 + 1;
		i++;
	}
	return i;
}
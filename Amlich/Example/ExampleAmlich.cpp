#include <Amlich.h>
int main() {
	// your code goes here
	NgayThangNam current = NgayThangNam(26,7,17);
	NgayThangNam ncurrent = current.ConvertDay();
	cout <<"Ngay: "<<(unsigned int) ncurrent.ngay<<endl<<"Thang: "<< (int)ncurrent.thang<<endl<<"Nam: "<<(int)ncurrent.nam<<endl;
	return 0;
}
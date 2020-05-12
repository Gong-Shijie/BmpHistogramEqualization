
#include "Bitmap.h"
#include <iostream>

int main()
{
	
		
		Bitmap bmp;
		if (!bmp.ReadBmp("C:\\users\\Gongshijie\\Desktop\\e.bmp")) {
			cout << "打开文件失败！" << endl;
		}
		cout << "打开文件成功！" << endl;

		if (bmp.bi.biBitCount < 24) {
			bmp.GrayDrawHistogram();
			bmp.GrayHistogramEqualization();
			bmp.GrayDrawHistogram();
		}
		else {
			bmp.ColorDrawHistogram();
			bmp.ColorHistogramEqualization();
			bmp.ColorDrawHistogram();
		}

	
		bmp.SaveBmp("C:\\users\\Gongshijie\\Desktop\\eOutPut.bmp");


	return 0;
}
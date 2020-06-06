
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

			bmp.GrayHistogramEqualization();

		}
		else {

			bmp.ColorHistogramEqualization();

		}

	
		bmp.SaveBmp("C:\\users\\Gongshijie\\Desktop\\eOutPut.bmp");


	return 0;
}
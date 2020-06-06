#include <iostream>
#include <string.h>
#include <stdio.h>
#include <Windows.h>
#include<math.h>

using namespace std;

const double PI = acos(-1);

#pragma once
class Bitmap {
public:
	/*********************************************************
	* 构造函数
	*********************************************************/
	Bitmap() {};
	Bitmap(Bitmap& bmp);

	/*********************************************************
	* 文件操作
	*********************************************************/
	bool ReadBmp(const char *FileName);
	bool SaveBmp(const char *FileName);

	/*********************************************************
	* 直方图均衡
	*********************************************************/

	void GrayHistogramEqualization();
	void ColorHistogramEqualization();

public:
	BITMAPFILEHEADER bf;			//文件头
	BITMAPINFOHEADER bi;			//信息头
	RGBQUAD *rgbPalette;			//调色板
	int sizeOfHang;					//一行所占字节数
	void** img;						//存放图片信息
};


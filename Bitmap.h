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
	void GrayDrawHistogram();
	void ColorDrawHistogram();
	void GrayHistogramEqualization();
	void ColorHistogramEqualization();

public:
	BITMAPFILEHEADER bf;			//文件头
	BITMAPINFOHEADER bi;			//信息头
	RGBQUAD *rgbPalette;			//调色板
	int sizeOfHang;					//一行所占字节数
	void** img;						//存放图片信息
};



/*********************************************************
* 构造函数
*********************************************************/

Bitmap::Bitmap(Bitmap & bmp) {
	bf = bmp.bf;
	bi = bmp.bi;
	sizeOfHang = bmp.sizeOfHang;

	rgbPalette = new RGBQUAD[1 << bi.biBitCount];
	memcpy(rgbPalette, bmp.rgbPalette, sizeof(RGBQUAD)*(1 << bi.biBitCount));

	img = new void*[bi.biHeight];							//图像数据行数
	for (int i = 0; i < bi.biHeight; i++) {
		img[i] = (void *) new unsigned char[sizeOfHang];	//为每行分配空间
		memcpy(img[i], bmp.img[i], sizeOfHang);
	}
}

/*********************************************************
* 文件操作
*********************************************************/
bool Bitmap::ReadBmp(const char *FileName) {
	//打开文件
	FILE *fpIn;
	if (fopen_s(&fpIn, FileName, "rb")) //fopen_s打开文件成功返回0，失败返回错误代码
		return false; //打开文件失败

	//读入文件头和信息头
	fread(&bf, sizeof(BITMAPFILEHEADER), 1, fpIn);
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, fpIn);

	//读入可能存在的调色板
	if (bi.biBitCount < 16) {
		int sizeOfPalette = 1 << bi.biBitCount;						//调色板大小
		rgbPalette = new RGBQUAD[sizeOfPalette];					//分配数组空间
		fread(rgbPalette, sizeof(RGBQUAD), sizeOfPalette, fpIn);	//读入调色板
	}

	//计算一行 像素  所占的字节数  为 4 的 整数 倍
	sizeOfHang = (bi.biBitCount * bi.biWidth + 31) / 32 * 4;

	cout << bi.biBitCount<<" "<<bi.biWidth << " " << bi.biHeight << endl;
	bi.biWidth = abs(bi.biWidth);
	bi.biHeight = abs(bi.biHeight);

	//读入每一行的像素值
	img = new void*[bi.biHeight];									//图像数据行数
	for (int i = 0; i < bi.biHeight; i++) {
		img[i] = (void *) new BYTE[sizeOfHang];						//为每行分配空间
		fread(img[i], sizeOfHang, 1, fpIn);
	}

	//关闭文件
	if (fpIn) fclose(fpIn);

	//读入文件成功
	return true;
}

bool Bitmap::SaveBmp(const char *FileName) {
	//打开文件
	FILE *fpOut;
	if (fopen_s(&fpOut, FileName, "wb")) return false;

	//写入文件头和信息头
	fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, fpOut);
	fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, fpOut);

	//写入可能存在的调色板
	if (bi.biBitCount < 16) {
		int sizeOfPalette = 1 << bi.biBitCount;						//调色板大小
		fwrite(rgbPalette, sizeof(RGBQUAD), sizeOfPalette, fpOut);
	}

	//写入像素值信息
	for (int i = 0; i < bi.biHeight; i++)
		fwrite(img[i], sizeOfHang, 1, fpOut);

	//关闭文件
	if (fpOut) fclose(fpOut);

	//写入文件成功
	return true;
}

/*********************************************************
* 直方图均衡
*********************************************************/

void Bitmap::GrayDrawHistogram() {
	const int numberOfCol = 55;
	const int numberOfRow = 40;

	int cnt1[256]; //计算每个像素值的出现次数
	for (int i = 0; i < 256; ++i) cnt1[i] = 0;
	for (int i = 0; i < bi.biHeight; ++i)
		for (int j = 0; j < bi.biWidth; ++j) {
			int pix = (int)(((BYTE*)img[i])[j]);
			cnt1[pix]++;
		}

	int cnt2[numberOfCol]; //计算每列对应的像素值的出现次数
	int size = (256 + numberOfCol - 1) / numberOfCol;
	for (int i = 0; i < numberOfCol; ++i) cnt2[i] = 0;
	for (int i = 0; i < 256; ++i) cnt2[(i + 1) / size] += cnt1[i];

	int maxV = 0;
	for (int i = 0; i < numberOfCol; ++i) maxV = max(maxV, cnt2[i]);

	for (int i = numberOfRow; i >= 0; --i) {
		for (int j = 0; j < numberOfCol; ++j) {
			//把maxV出现的列拉满，其余列按比例分布
			if (cnt2[j] * numberOfRow >= maxV * i) cout << "■";
			else cout << "  ";
		}
		cout << endl;
	}
}

void Bitmap::ColorDrawHistogram() {
	const int numberOfCol = 110;
	const int numberOfRow = 40;

	int cnt1[256]; //计算每个像素值的出现次数
	for (int i = 0; i < 256; ++i) cnt1[i] = 0;
	for (int i = 0; i < bi.biHeight; ++i)
		for (int j = 0; j < bi.biWidth; ++j) {
			int b = (int)(((BYTE*)img[i])[j * 3]);
			int g = (int)(((BYTE*)img[i])[j * 3 + 1]);
			int r = (int)(((BYTE*)img[i])[j * 3 + 2]);
			int I = (r + g + b) / 3;
			cnt1[I]++;
		}

	int cnt2[numberOfCol]; //计算每列对应的像素值的出现次数
	int size = (256 + numberOfCol - 1) / numberOfCol;
	for (int i = 0; i < numberOfCol; ++i) cnt2[i] = 0;
	for (int i = 0; i < 256; ++i) cnt2[(i + 1) / size] += cnt1[i];

	int maxV = 0;
	for (int i = 0; i < numberOfCol; ++i) maxV = max(maxV, cnt2[i]);

	for (int i = numberOfRow; i >= 0; --i) {
		for (int j = 0; j < numberOfCol; ++j) {
			//把maxV出现的列拉满，其余列按比例分布
			if (cnt2[j] * numberOfRow >= maxV * i) cout << "■";
			else cout << "  ";
		}
		cout << endl;
	}
}

//灰度图直方图均衡
void Bitmap::GrayHistogramEqualization() {
	const int L = 256; //像素值的取值范围为[0, L)
	int R[L]; //原图中各像素值出现的次数的前缀和
	int S[L]; //直方图均衡后像素的对应值，原像素值为i的值对应到S[i]

	//统计每个像素值出现次数
	for (int i = 0; i < L; ++i) R[i] = 0;
	for (int i = 0; i < bi.biHeight; ++i)
		for (int j = 0; j < bi.biWidth; ++j) {
			int pix = (int)(((BYTE*)img[i])[j]);
			R[pix]++;
		}
	//维护R数组前缀和
	for (int i = 1; i < L; ++i) R[i] += R[i - 1];

	//计算S数组
	for (int i = 0; i < L; ++i)
		S[i] = (int)(1.0 * (L - 1) * R[i] / R[L - 1]);

	//对原图进行直方图均衡
	for (int i = 0; i < bi.biHeight; ++i)
		for (int j = 0; j < bi.biWidth; ++j) {
			int pix = (int)(((BYTE*)img[i])[j]);
			pix = S[pix];
			((BYTE*)img[i])[j] = (BYTE)pix;
		}
}

//彩色图直方图均衡
void Bitmap::ColorHistogramEqualization() {
	const int L = 256; //像素值的取值范围为[0, L)
	int R[L]; //原图中各像素值出现的次数的前缀和
	int S[L]; //直方图均衡后像素的对应值，原像素值为i的值对应到S[i]

	//统计每个像素值出现次数
	for (int i = 0; i < L; ++i) R[i] = 0;
	for (int i = 0; i < bi.biHeight; ++i)
		for (int j = 0; j < bi.biWidth; ++j) {
			//RGB通道转为HSI通道的I分量（亮度）值
			int b = (int)(((BYTE*)img[i])[j * 3]);
			int g = (int)(((BYTE*)img[i])[j * 3 + 1]);
			int r = (int)(((BYTE*)img[i])[j * 3 + 2]);
			int I = (r + g + b) / 3;
			R[I]++;
		}
	//维护R数组前缀和
	for (int i = 1; i < L; ++i) R[i] += R[i - 1];

	//计算S数组
	for (int i = 0; i < L; ++i)
		S[i] = (int)(1.0 * (L - 1) * R[i] / R[L - 1]);

	//对原图进行直方图均衡
	for (int i = 0; i < bi.biHeight; ++i)
		for (int j = 0; j < bi.biWidth; ++j) {
			int b = (int)(((BYTE*)img[i])[j * 3]);
			int g = (int)(((BYTE*)img[i])[j * 3 + 1]);
			int r = (int)(((BYTE*)img[i])[j * 3 + 2]);

			double theta = (r == g && g == b) ? 0 : acos(0.5 * (r - g + r - b) / sqrt((r - g)*(r - g) + (r - b)*(g - b)));
			double I = S[(r + g + b) / 3];//这是经过直方图均衡过的I
			double H = b > g ? PI + PI - theta : theta;
			double S = 1 - 3.0 * min(r, min(g, b)) / (r + g + b);

			//用新的I值返回得到原RGB值
			if (H < PI * 2 / 3) {
				b = I * (1 - S);
				r = I * (1 + S * cos(H) / cos(PI / 3 - H));
				g = 3 * I - r - b;
			}
			else if (H < PI * 4 / 3) {
				H -= PI * 2 / 3;
				r = I * (1 - S);
				g = I * (1 + S * cos(H) / cos(PI / 3 - H));
				b = 3 * I - r - g;
			}
			else {
				H -= PI * 4 / 3;
				g = I * (1 - S);
				b = I * (1 + S * cos(H) / cos(PI / 3 - H));
				r = 3 * I - g - b;
			}

			r = min(255, r); g = min(255, g); b = min(255, b);

			((BYTE*)img[i])[j * 3] = (BYTE)b;
			((BYTE*)img[i])[j * 3 + 1] = (BYTE)g;
			((BYTE*)img[i])[j * 3 + 2] = (BYTE)r;
		}
}
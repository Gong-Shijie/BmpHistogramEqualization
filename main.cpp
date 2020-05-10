#include "SaveLoad.h"

int main() {
    BmpImage bmpImage;
    bmpImage.LoadImage(R"(C:\Users\Gongshijie\Desktop\pepper.bmp)");
    bmpImage.ShowBMPInfo();
    bmpImage.SaveImage();
    return 0;
}

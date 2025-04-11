#include "header/imageio.hpp"
using namespace std;

FIBITMAP* loadImage(const std::string& filename, int& width, int& height) {
    FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(filename.c_str(), 0);
    if (fif == FIF_UNKNOWN) fif = FreeImage_GetFIFFromFilename(filename.c_str());
    if (fif == FIF_UNKNOWN) return nullptr;

    FIBITMAP* dib = FreeImage_Load(fif, filename.c_str());
    if (!dib) return nullptr;

    FIBITMAP* rgbImage = FreeImage_ConvertTo24Bits(dib);
    FreeImage_Unload(dib);

    width = FreeImage_GetWidth(rgbImage);
    height = FreeImage_GetHeight(rgbImage);
    return rgbImage;
}

bool saveImage(const std::string& filename, FIBITMAP* image) {
    return FreeImage_Save(FIF_PNG, image, filename.c_str(), PNG_DEFAULT);
}

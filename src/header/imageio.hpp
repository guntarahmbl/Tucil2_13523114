#ifndef IMAGEIO_HPP
#define IMAGEIO_HPP

#include <FreeImage.h>
#include <string>

FIBITMAP* loadImage(const std::string& filename, int& width, int& height);
bool saveImage(const std::string& filename, FIBITMAP* image);

#endif

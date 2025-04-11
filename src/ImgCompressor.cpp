#include "header/ImgCompressor.hpp"
#include "header/gif.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <FreeImage.h>
#include <sys/stat.h>


using namespace std;

#define GIF_TEMP_MALLOC malloc
#define GIF_TEMP_FREE free

ImgCompressor::ImgCompressor(const string& inPath, const string& outPath, int m, double t, int minSize, double target)
    : inputPath(inPath), outputPath(outPath), method(m), threshold(t), minBlockSize(minSize),
      targetCompression(target), width(0), height(0), channels(3),
      originalData(nullptr), root(nullptr),
      originalSize(0), compressedSize(0), treeDepth(0), nodeCount(0) {}


ImgCompressor::~ImgCompressor() {
    if (originalData) FreeImage_Unload((FIBITMAP*)originalData);
    if (root) delete root;
}

static size_t getFileSize(const string& filename) {
    struct stat stat_buf;
    return stat(filename.c_str(), &stat_buf) == 0 ? stat_buf.st_size : 0;
}

double ImgCompressor::findBestThreshold() {
    double low = 0.0, high = 100.0;
    double bestThreshold = threshold;
    double epsilon = 0.01;
    int maxIter = 20;

    for (int i = 0; i < maxIter; ++i) {
        double mid = (low + high) / 2.0;
        threshold = mid;
        double ratio = runCompression(threshold);

        if (abs(ratio - targetCompression) < epsilon) {
            bestThreshold = mid;
            break;
        }

        if (ratio < targetCompression) {
            low = mid;
        } else {
            high = mid;
        }

        bestThreshold = mid;
    }

    return bestThreshold;
}

double ImgCompressor::runCompression(double currentThreshold) {
    if (root) delete root;
    root = new QuadtreeNode(0, 0, width, height);
    BYTE* bits = FreeImage_GetBits(originalData);

    root->extractNode(bits, width, height);
    divide(root, currentThreshold);

    FIBITMAP* outImage = FreeImage_Allocate(width, height, 24);
    BYTE* outBits = FreeImage_GetBits(outImage);

    root->renderNode(outBits, width, height);
    saveImage(outputPath, outImage);

    FreeImage_Unload(outImage);
    compressedSize = getFileSize(outputPath);

    return getCompressionRatio();
}

void ImgCompressor::process() {
    FreeImage_Initialise();

    FIBITMAP* image = loadImage(inputPath, width, height);
    if (!image) {
        cerr << "Failed to load image using FreeImage." << endl;
        return;
    }

    originalData = image;
    originalSize = getFileSize(inputPath);

    if (targetCompression > 0.0) {
        threshold = findBestThreshold();
    }

    runCompression(threshold);
    FreeImage_DeInitialise();
}

void ImgCompressor::divide(QuadtreeNode* node, double currentThreshold) {
    nodeCount++;
    treeDepth = max(treeDepth, node->depth);

    double error = computeError(node->pixels, method);
    bool canSplit = (node->height * node->width / 4 >= minBlockSize);

    if (error <= currentThreshold || !canSplit) {
        return; 
    }

    node->splitBlock();
    for (auto* child : node->children) {
        child->extractNode(FreeImage_GetBits((FIBITMAP*)originalData), width, height);
        divide(child, currentThreshold); 
    }
}

void ImgCompressor::generateGif(const string& gifPath, int duration) {
    int delay = duration / 10;

    GifWriter writer = {};
    GifBegin(&writer, gifPath.c_str(), width, height, delay);

    for (int d = 0; d <= treeDepth; ++d) {
        FIBITMAP* frameImage = FreeImage_Allocate(width, height, 24);
        BYTE* frameBits = FreeImage_GetBits(frameImage);
        root->renderAtDepth(frameBits, width, height, d);

        std::vector<uint8_t> rgba(width * height * 4);
        for (int y = 0; y < height; ++y) {
            int flippedY = height - 1 - y;  // mirror correction
            for (int x = 0; x < width; ++x) {
                int i = flippedY * width + x;
                BYTE* pixel = frameBits + i * 3;
                int j = y * width + x;
                rgba[j * 4 + 0] = pixel[FI_RGBA_RED];
                rgba[j * 4 + 1] = pixel[FI_RGBA_GREEN];
                rgba[j * 4 + 2] = pixel[FI_RGBA_BLUE];
                rgba[j * 4 + 3] = 255;
            }
        }

        GifWriteFrame(&writer, rgba.data(), width, height, delay);
        FreeImage_Unload(frameImage);
    }

    // Frame akhir (hasil kompresi)
    for (int i = 0; i < 4; ++i) {
        FIBITMAP* finalImage = FreeImage_Allocate(width, height, 24);
        BYTE* finalBits = FreeImage_GetBits(finalImage);
        root->renderNode(finalBits, width, height);

        std::vector<uint8_t> rgba(width * height * 4);
        for (int y = 0; y < height; ++y) {
            int flippedY = height - 1 - y;  // mirror correction
            for (int x = 0; x < width; ++x) {
                int i = flippedY * width + x;
                BYTE* pixel = finalBits + i * 3;
                int j = y * width + x;
                rgba[j * 4 + 0] = pixel[FI_RGBA_RED];
                rgba[j * 4 + 1] = pixel[FI_RGBA_GREEN];
                rgba[j * 4 + 2] = pixel[FI_RGBA_BLUE];
                rgba[j * 4 + 3] = 255;
            }
        }

        GifWriteFrame(&writer, rgba.data(), width, height, delay);
        FreeImage_Unload(finalImage);
    }

    GifEnd(&writer);
}

void ImgCompressor::printStats(double execTime) const {
    std::cout << "\n=== COMPRESSION STATS ===\n";
    std::cout << "Execution Time        : " << execTime << " seconds\n";
    std::cout << "Original File Size    : " << originalSize << "\n";
    std::cout << "Compressed File Size  : " << compressedSize << "\n";
    std::cout << "Compression Ratio     : " << std::fixed << std::setprecision(2) << getCompressionRatio() * 100 << " %\n";
    std::cout << "Tree Depth            : " << treeDepth << "\n";
    std::cout << "Tree Nodes            : " << nodeCount << "\n";
    std::cout << "Compressed image saved to: " << outputPath << "\n";
}

size_t ImgCompressor::getOriginalSize() const {
    return originalSize;
}

size_t ImgCompressor::getCompressedSize() const {
    return compressedSize;
}

double ImgCompressor::getCompressionRatio() const {
    return 1 - (double)compressedSize / (double)originalSize;
}

int ImgCompressor::getTreeDepth() const {
    return treeDepth;
}

int ImgCompressor::getNodeCount() const {
    return nodeCount;
}
#ifndef IMGCOMPRESSOR_HPP
#define IMGCOMPRESSOR_HPP

#include <string>
#include <vector>
#include <cstdint>
#include "QuadtreeNode.hpp"
#include "metrics.hpp"
#include "imageio.hpp"

class ImgCompressor {
private:
    std::string inputPath, outputPath;
    int method;
    double threshold;
    int minBlockSize;
    double targetCompression;

    int width, height, channels;
    FIBITMAP* originalData;
    QuadtreeNode* root;

    size_t originalSize;
    size_t compressedSize;
    int treeDepth;
    int nodeCount;

public:
    ImgCompressor(const std::string& inPath, const std::string& outPath, int m, double t, int minSize, double target);
    ~ImgCompressor();

    double findBestThreshold();
    double runCompression(double currentThreshold);
    void process();
    void divide(QuadtreeNode* node, double currentThreshold);
    void generateGif(const std::string& gifPath, int duration);

    size_t getOriginalSize() const;
    size_t getCompressedSize() const;
    double getCompressionRatio() const;
    int getTreeDepth() const;
    int getNodeCount() const;

    void printStats(double execTimeInSeconds) const;
};

#endif

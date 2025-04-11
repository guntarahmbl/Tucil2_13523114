#include "header/QuadtreeNode.hpp"
#include <numeric>
#include <algorithm>

void QuadtreeNode::extractNode(const unsigned char* data, int imgWidth, int imgHeight) {
    pixels.clear();
    long long sumR = 0, sumG = 0, sumB = 0;
    int count = 0;

    for (int j = y; j < y + height && j < imgHeight; ++j) {
        for (int i = x; i < x + width && i < imgWidth; ++i) {
            int idx = (j * imgWidth + i) * 3;
            RGB color = { data[idx], data[idx + 1], data[idx + 2] };
            pixels.push_back(color);
            sumR += color.r;
            sumG += color.g;
            sumB += color.b;
            count++;
        }
    }
    averageColor.r = sumR / count;
    averageColor.g = sumG / count;
    averageColor.b = sumB / count;
}

void QuadtreeNode::splitBlock() {
    int halfW = width / 2;
    int halfH = height / 2;
    children.push_back(new QuadtreeNode(x, y, halfW, halfH, depth + 1)); // atas kiri
    children.push_back(new QuadtreeNode(x + halfW, y, width - halfW, halfH, depth + 1)); // atas kanan
    children.push_back(new QuadtreeNode(x, y + halfH, halfW, height - halfH, depth + 1)); // bawah kiri
    children.push_back(new QuadtreeNode(x + halfW, y + halfH, width - halfW, height - halfH, depth + 1)); // bawah kanan
}

void QuadtreeNode::renderNode(unsigned char* output, int imgWidth, int imgHeight) const {
    if (children.empty()) {
        for (int j = y; j < y + height && j < imgHeight; ++j) {
            for (int i = x; i < x + width && i < imgWidth; ++i) {
                int idx = (j * imgWidth + i) * 3;
                output[idx] = averageColor.r;
                output[idx + 1] = averageColor.g;
                output[idx + 2] = averageColor.b;
            }
        }
    } else {
        for (auto* child : children) {
            child->renderNode(output, imgWidth, imgHeight);
        }
    }
}

void QuadtreeNode::renderAtDepth(unsigned char* output, int imgWidth, int imgHeight, int maxDepth) const {
    if (depth > maxDepth) return;

    if (children.empty() || depth == maxDepth) {
        for (int j = y; j < y + height && j < imgHeight; ++j) {
            for (int i = x; i < x + width && i < imgWidth; ++i) {
                int idx = (j * imgWidth + i) * 3;
                output[idx]     = averageColor.r;
                output[idx + 1] = averageColor.g;
                output[idx + 2] = averageColor.b;
            }
        }
    } else {
        for (auto* child : children) {
            child->renderAtDepth(output, imgWidth, imgHeight, maxDepth);
        }
    }
}


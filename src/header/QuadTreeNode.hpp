#ifndef QUADTREE_NODE_HPP
#define QUADTREE_NODE_HPP

#include <vector>
#include <cstdint>
#include <FreeImage.h>

struct RGB {
    uint8_t r, g, b;
};

class QuadtreeNode {
public:
    int x, y, width, height;
    int depth;
    std::vector<RGB> pixels;
    std::vector<QuadtreeNode*> children;
    RGB averageColor;

    QuadtreeNode(int x, int y, int w, int h, int d = 0)
        : x(x), y(y), width(w), height(h), depth(d) {}

    ~QuadtreeNode() {
        for (auto* child : children) {
            delete child;
        }
    }

    void extractNode(const unsigned char* data, int imgWidth, int imgHeight);
    void splitBlock();
    void renderNode(unsigned char* output, int imgWidth, int imgHeight) const;
    void renderAtDepth(BYTE* bits, int width, int height, int maxDepth) const;
};

#endif
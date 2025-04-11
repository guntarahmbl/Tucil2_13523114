#include "header/QuadtreeNode.hpp"
#include "header/metrics.hpp"


double computeVariance(const std::vector<RGB>& pixels) {
    double sumR = 0, sumG = 0, sumB = 0;
    for (const auto& p : pixels) {
        sumR += p.r;
        sumG += p.g;
        sumB += p.b;
    }
    double n = pixels.size();
    double meanR = sumR / n;
    double meanG = sumG / n;
    double meanB = sumB / n;

    double varR = 0;
    double varG = 0;
    double varB = 0;
    for (const auto& p : pixels) {
        varR += pow(p.r - meanR, 2);
        varG += pow(p.g - meanG, 2);
        varB += pow(p.b - meanB, 2);
    }
    varR /= n;
    varG /= n;
    varB /= n;
    return (varR + varG + varB) / 3;
}

double computeMAD(const std::vector<RGB>& pixels) {
    double sumR = 0, sumG = 0, sumB = 0;
    for (const auto& p : pixels) {
        sumR += p.r;
        sumG += p.g;
        sumB += p.b;
    }
    double n = pixels.size();
    double meanR = sumR / n;
    double meanG = sumG / n;
    double meanB = sumB / n;

    double MADR = 0;
    double MADG = 0;
    double MADB = 0;
    for (const auto& p : pixels) {
        MADR += abs(p.r - meanR);
        MADG += abs(p.g - meanG);
        MADB += abs(p.b - meanB);
    }
    MADR /= n;
    MADG /= n;
    MADB /= n;
    return (MADR + MADG + MADB) / 3;
}

double computeEntropy(const std::vector<RGB>& pixels) {
    std::array<int, 256> histR{}, histG{}, histB{};

    for (const auto& p : pixels) {
        histR[p.r]++;
        histG[p.g]++;
        histB[p.b]++;
    }

    double n = static_cast<double>(pixels.size());
    auto channelEntropy = [n](const std::array<int, 256>& hist) -> double {
        double entropy = 0.0;
        for (int count : hist) {
            if (count == 0) continue;
            double p = count / n;
            entropy -= p * std::log2(p);
        }
        return entropy;
    };

    double hR = channelEntropy(histR);
    double hG = channelEntropy(histG);
    double hB = channelEntropy(histB);

    return (hR + hG + hB) / 3.0;
}

double computeMaxDiff(const std::vector<RGB>& pixels) {
    uint8_t minR = 255, minG = 255, minB = 255;
    uint8_t maxR = 0, maxG = 0, maxB = 0;
    for (const auto& p : pixels) {
        minR = std::min(minR, p.r);
        minG = std::min(minG, p.g);
        minB = std::min(minB, p.b);

        maxR = std::max(maxR, p.r);
        maxG = std::max(maxG, p.g);
        maxB = std::max(maxB, p.b);
    }
    return ((maxR - minR) + (maxG - minG) + (maxB - minB)) / 3;
}

double computeError(const std::vector<RGB>& pixels, int method) {
    switch (method) {
        case 1: return computeVariance(pixels);
        case 2: return computeMAD(pixels);
        case 3: return computeEntropy(pixels);
        case 4: return computeMaxDiff(pixels);
        default: return 0.0;
    }
}


#ifndef METRICS_HPP
#define METRICS_HPP

#include <vector>
#include <cmath>
#include <array>
#include "QuadtreeNode.hpp"

using namespace std;
double computeVariance(const vector<RGB>& pixels);
double computeMAD(const vector<RGB>& pixels);
double computeEntropy(const vector<RGB>& pixels);
double computeMaxDiff(const vector<RGB>& pixels);
double computeError(const vector<RGB>& pixels, int method);

#endif

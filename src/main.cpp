#include <iostream>
#include <string>
#include <chrono>
#include "header/ImgCompressor.hpp"
using namespace std;

int main() {
    string inputPath, outputPath, outputGifPath;
    int method, minBlockSize;
    double threshold, targetCompression;

    cout << "=== Kompresi GAMBAR MENGGUNAKAN QUADTREE ===\n";

    cout << "Masukkan path absolut gambar: ";
    getline(cin, inputPath);

    cout << "Pilih metode perhitungan error:\n";
    cout << "[1] Variance\n[2] Mean Absolute Deviation (MAD)\n[3] Entropy\n[4] Max Pixel Difference\n";
    cout << "Masukkan nomor metode: ";
    cin >> method;

    cout << "Masukkan nilai threshold (berdasarkan metode): ";
    cin >> threshold;

    cout << "Masukkan ukuran blok minimum: ";
    cin >> minBlockSize;

    cout << "Masukkan target rasio kompresi (0 untuk menonaktifkan): ";
    cin >> targetCompression;
    cin.ignore();

    cout << "Masukkan path absolut gambar hasil kompresi: ";
    getline(cin, outputPath);

    auto start = chrono::high_resolution_clock::now();

    ImgCompressor compressor(inputPath, outputPath, method, threshold, minBlockSize, targetCompression);
    compressor.process();

    cout << "Masukkan path absolut GIF: ";
    getline(cin, outputGifPath);
    compressor.generateGif(outputGifPath, 1000);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> execTime = end - start;

    compressor.printStats(execTime.count());

    return 0;
}

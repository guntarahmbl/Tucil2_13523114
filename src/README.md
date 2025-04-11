# Image Compressor (Using Quadtree)
Image Compressor merupakan sebuah program yang dapat digunakan untuk memperkecil ukuran gambar. Program ini menggunakan struktur data Quadtree untuk merepresentasikan sebuah blok (yang akan dibagi atau tidak berdasarkan metrik error tertentu). Pembagian (split) blok gambar ini merupakan salah satu penerapan dari algoritma Divide and Conquer.

## Struktur Folder
```
ImageCompressor/
├── bin/
│   ├── output.exe
├── doc/
├── src/
|   ├── header/
│   |   ├── gif.h
│   |   ├── imageio.cpp
│   |   ├── ImgCompressor.hpp
│   |   ├── metrics.hpp
│   |   ├── QuadTreeNode.hpp
│   ├── main.cpp
│   ├── ImgCompressor.cpp
│   ├── metrics.cpp
│   ├── imageio.cpp
│   ├── QuadtreeNode.cpp
├── test/
└── README.md
```

## Requirement
1. Requirement yang diperlukan (jika ingin lakukan kompilasi, secara default tidak perlu) adalah library FreeImage dari sumber di bawah
https://freeimage.sourceforge.io/download.html 

2. Ekstrak zip, lalu buka hasil ekstraksi
```bash
cd FreeImage\Dist\x64
```
3. Pindahkan isi folder tersebut ke Folder lain, misalnya
D:\Libraries\FreeImage

4. Tempatkan sehingga struktur folder sbb:
```
FreeImage/
├── bin/
│   ├── FreeImage.dll
├── include/
│   ├── FreeImage.h
├── lib/
│   ├── FreeImage.lib
```


## Instalasi dan Kompilasi
1. **Clone repository**
Seluruh requirement sudah diberikan di repository
```bash
git clone https://github.com/guntarahmbl/Tucil2_13523114.git
```

2. **Pergi ke directory /Tucil2_13523114/src**

```bash
  cd Tucil2_13523114/src
```

3. **Lakukan kompilasi (secara default tidak perlu kompilasi karena sudah ada .exe di bin folder)**

```bash
g++ -O2 main.cpp ImgCompressor.cpp QuadTreeNode.cpp metrics.cpp imageio.cpp -I"D:/Libraries/FreeImage/include" -L"D:/Libraries/FreeImage/lib" -o ../bin/output.exe -lfreeimage -Wall -Wextra
```


## Cara Penggunaan
1. **Jalankan Program (dari root src)**
```bash
../bin/output.exe 
```
2. **Masukkan input yang dibutuhkan sesuai contoh di bawah. Pastikan file image ada**

## Contoh Input
=== Kompresi GAMBAR MENGGUNAKAN QUADTREE ===
Masukkan path absolut gambar: D:/tree.jpg   
Pilih metode perhitungan error:
[1] Variance
[2] Mean Absolute Deviation (MAD)
[3] Entropy
[4] Max Pixel Difference
Masukkan nomor metode: 2
Masukkan nilai threshold (berdasarkan metode): 9.375
Masukkan ukuran blok minimum: 8
Masukkan target rasio kompresi (0 untuk menonaktifkan): 0
Masukkan path absolut gambar hasil kompresi: D:/treeMAD.jpg
Masukkan path absolut GIF: D:/treeMAD.gif

## Author
| Guntara Hambali  13523114  K02 |

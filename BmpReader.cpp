#include "BmpReader.h"
#include <algorithm>

bool BmpReader::openBmp(const std::string& fileName)
{
    fileStream.open(fileName, std::ios::binary);
    if (!fileStream) {
        std::cerr << "Ошибка при открытии файла: " << fileName << std::endl;
        return false;
    }

    fileStream.read(reinterpret_cast<char*>(&bmpFileHeader), sizeof(BITMAPFILEHEADER));
    fileStream.read(reinterpret_cast<char*>(&bmpInfoHeader), sizeof(BITMAPINFOHEADER));

    if (bmpFileHeader.bfType != 0x4D42) {
        std::cerr << "Файл не является BMP: " << fileName << std::endl;
        return false;
    }

    if (bmpInfoHeader.biBitCount != 24 && bmpInfoHeader.biBitCount != 32) {
        std::cerr << "Поддерживаются только 24 или 32-битные BMP изображения" << std::endl;
        return false;
    }

    bool flip = true;
    if (bmpInfoHeader.biHeight < 0) {
        flip = false;
        bmpInfoHeader.biHeight = -bmpInfoHeader.biHeight;
    }

    width = bmpInfoHeader.biWidth;
    height = bmpInfoHeader.biHeight;
    imageData.resize(height, std::vector<bool>(width));

    fileStream.seekg(bmpFileHeader.bfOffBits, std::ios::beg);

    int rowSize = ((bmpInfoHeader.biBitCount * width + 31) / 32) * 4;
    std::vector<char> rowData(rowSize);

    for (int i = 0; i < height; ++i) {
        fileStream.read(rowData.data(), rowSize);

        for (int j = 0; j < width; ++j) {
            int pixelOffset = j * (bmpInfoHeader.biBitCount / 8);
            unsigned char blue = rowData[pixelOffset];
            unsigned char green = rowData[pixelOffset + 1];
            unsigned char red = rowData[pixelOffset + 2];

            if (red == 0 && green == 0 && blue == 0) {
                imageData[i][j] = true;
            }
            else if (red == 255 && green == 255 && blue == 255) {
                imageData[i][j] = false;
            }
            else {
                std::cerr << "Ошибка: изображение содержит цвета, отличные от черного и белого" << std::endl;
                return false;
            }
        }
    }

    if (flip) {
        std::reverse(imageData.begin(), imageData.end());
    }

    return true;
}

void BmpReader::displayBmp()
{
    if (imageData.empty()) {
        std::cerr << "Нет данных для отображения" << std::endl;
        return;
    }

    for (const auto& row : imageData) {
        for (bool isBlack : row) {
            if (isBlack) {                                 
                std::cout << "@@";
            }
            else {
                std::cout << "  ";
            }
        }
        std::cout << std::endl;
    }
}

void BmpReader::closeBmp()
{
    if (fileStream.is_open()) {
        fileStream.close();
    }
    imageData.clear();
}

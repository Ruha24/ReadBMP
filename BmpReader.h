#pragma once

#include <string>
#include <Windows.h>
#include <fstream>
#include <vector>
#include <iostream>

class BmpReader
{
public:
     BmpReader() : width(0), height(0) {}

	 bool openBmp(const std::string& fileName);

	 void displayBmp();

	 void closeBmp();

private:
    std::ifstream fileStream;
    BITMAPFILEHEADER bmpFileHeader;
    BITMAPINFOHEADER bmpInfoHeader;
    std::vector<std::vector<bool>> imageData;
    int width;
    int height;
};


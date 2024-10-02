#include "BmpReader.h"
#include <clocale>


int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "ru");

    if (argc != 2) {
        std::cerr << "Использование: drawBmp.exe <путь к файлу BMP>" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];
    BmpReader bmpReader;

    if (!bmpReader.openBmp(fileName)) {
        return 1;
    }

    bmpReader.displayBmp();
    bmpReader.closeBmp();

    return 0;
}
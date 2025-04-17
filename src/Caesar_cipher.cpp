#include "Caesar_cipher.h"
#include <iostream>
#include <fstream>
#include <stdexcept> 

void Caesar_cipher::encryptFile(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::cout << "Начало шифрования файла шифром Цезаря" << std::endl;

    std::ifstream inputFile(inputFilePath, std::ios::binary);
    std::ofstream outputFile(outputFilePath, std::ios::binary);

    if (!inputFile.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для чтения: " + inputFilePath);
    }

    if (!outputFile.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + outputFilePath);
    }

    char byte;
    while (inputFile.get(byte)) {
        outputFile.put(static_cast<char>(static_cast<unsigned char>(byte) + password));
    }

    std::cout << "Конец шифрования файла шифром Цезаря" << std::endl;
}

void Caesar_cipher::decryptFile(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::cout << "Начало расшифровки файла шифром Цезаря" << std::endl;

    std::ifstream inputFile(inputFilePath, std::ios::binary);
    std::ofstream outputFile(outputFilePath, std::ios::binary);

    if (!inputFile.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для чтения: " + inputFilePath);
    }

    if (!outputFile.is_open()) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + outputFilePath);
    }

    char byte;
    while (inputFile.get(byte)) {
        outputFile.put(static_cast<char>(static_cast<unsigned char>(byte) - password));
    }

    std::cout << "Конец расшифровки файла шифром Цезаря" << std::endl;
}


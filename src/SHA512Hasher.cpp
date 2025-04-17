#include "SHA512Hasher.h"
#include <openssl/sha.h>
#include <fstream>
#include <sstream>
#include <iomanip>

void SHA512Hasher::encryptFile(const std::string& inputFilePath, const std::string& outputFilePath) {
    // Чтение файла
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    if (!inputFile) {
        throw std::runtime_error("Cannot open input file: " + inputFilePath);
    }

    // Инициализация контекста SHA-512
    SHA512_CTX context;
    if (!SHA512_Init(&context)) {
        throw std::runtime_error("SHA512 initialization failed");
    }

    // Добавляем соль если она есть
    if (!salt.empty()) {
        SHA512_Update(&context, salt.c_str(), salt.size());
    }

    // Чтение и хеширование файла по частям
    char buffer[4096];
    while (inputFile.read(buffer, sizeof(buffer))) {
        SHA512_Update(&context, buffer, inputFile.gcount());
    }
    SHA512_Update(&context, buffer, inputFile.gcount());

    // Получение хеша
    unsigned char hash[SHA512_DIGEST_LENGTH];
    SHA512_Final(hash, &context);

    // Запись хеша в выходной файл
    std::ofstream outputFile(outputFilePath);
    if (!outputFile) {
        throw std::runtime_error("Cannot open output file: " + outputFilePath);
    }

    // Преобразование в hex-строку
    for (int i = 0; i < SHA512_DIGEST_LENGTH; ++i) {
        outputFile << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
}

std::string SHA512Hasher::hashString(const std::string& input, const std::string& salt) {
    SHA512_CTX context;
    SHA512_Init(&context);

    if (!salt.empty()) {
        SHA512_Update(&context, salt.c_str(), salt.size());
    }
    

    SHA512_Update(&context, input.c_str(), input.size());
    

    unsigned char hash[SHA512_DIGEST_LENGTH];
    SHA512_Final(hash, &context);


    std::stringstream ss;
    for (int i = 0; i < SHA512_DIGEST_LENGTH; ++i) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }


    return ss.str();
}
#include "AES.h"
#include <fstream>
#include <iostream>

#define AES_KEY_SIZE 32  // Размер ключа для AES-256 (в байтах)
#define AES_BLOCK_SIZE 16 

// Конструктор с ключом в виде строки
AESCipher::AESCipher(const std::string& keyStr) {
    if (keyStr.size() != AES_KEY_SIZE) {
        throw std::invalid_argument("Invalid key size. AES-256 requires a 32-byte key.");
    }
    key.assign(keyStr.begin(), keyStr.end());
    generateIV();
}

// Конструктор с ключом в виде вектора байтов
AESCipher::AESCipher(const std::vector<unsigned char>& keyVec) {
    if (keyVec.size() != AES_KEY_SIZE) {
        throw std::invalid_argument("Invalid key size. AES-256 requires a 32-byte key.");
    }
    key = keyVec;
    generateIV();
}

// Метод для обработки ошибок OpenSSL
void AESCipher::handleErrors() {
    ERR_print_errors_fp(stderr);
    throw std::runtime_error("OpenSSL error occurred.");
}

// Генерация случайного IV (Initialization Vector)
void AESCipher::generateIV() {
    iv.resize(AES_BLOCK_SIZE);
    if (!RAND_bytes(iv.data(), AES_BLOCK_SIZE)) {
        handleErrors();
    }
}

// Чтение файла в буфер
size_t AESCipher::readFile(const std::string& filePath, std::vector<unsigned char>& buffer) {
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open file for reading: " + filePath);
    }

    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer.resize(fileSize);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
        throw std::runtime_error("Failed to read file: " + filePath);
    }

    return fileSize;
}

// Запись буфера в файл
void AESCipher::writeFile(const std::string& filePath, const std::vector<unsigned char>& buffer) {
    std::ofstream file(filePath, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Failed to open file for writing: " + filePath);
    }

    if (!file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size())) {
        throw std::runtime_error("Failed to write file: " + filePath);
    }
}

// Шифрование файла
void AESCipher::encryptFile(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::vector<unsigned char> plaintext;
    readFile(inputFilePath, plaintext);

    std::vector<unsigned char> ciphertext(plaintext.size() + AES_BLOCK_SIZE);
    int ciphertextLen;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        handleErrors();
    }

    // Инициализация контекста шифрования
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), iv.data())) {
        EVP_CIPHER_CTX_free(ctx);
        handleErrors();
    }

    // Основной процесс шифрования
    if (1 != EVP_EncryptUpdate(ctx, ciphertext.data(), &ciphertextLen, plaintext.data(), plaintext.size())) {
        EVP_CIPHER_CTX_free(ctx);
        handleErrors();
    }

    int finalLen;
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + ciphertextLen, &finalLen)) {
        EVP_CIPHER_CTX_free(ctx);
        handleErrors();
    }

    ciphertextLen += finalLen;
    ciphertext.resize(ciphertextLen);

    // Добавление IV в начало зашифрованных данных
    std::vector<unsigned char> encryptedData(iv.begin(), iv.end());
    encryptedData.insert(encryptedData.end(), ciphertext.begin(), ciphertext.end());

    writeFile(outputFilePath, encryptedData);

    EVP_CIPHER_CTX_free(ctx);
}

// Дешифрование файла
void AESCipher::decryptFile(const std::string& inputFilePath, const std::string& outputFilePath) {
    std::vector<unsigned char> encryptedData;
    readFile(inputFilePath, encryptedData);

    // Извлечение IV из начала файла
    std::vector<unsigned char> extractedIV(encryptedData.begin(), encryptedData.begin() + AES_BLOCK_SIZE);
    std::vector<unsigned char> ciphertext(encryptedData.begin() + AES_BLOCK_SIZE, encryptedData.end());

    std::vector<unsigned char> plaintext(ciphertext.size());
    int plaintextLen;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        handleErrors();
    }

    // Инициализация контекста дешифрования
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key.data(), extractedIV.data())) {
        EVP_CIPHER_CTX_free(ctx);
        handleErrors();
    }

    // Основной процесс дешифрования
    if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &plaintextLen, ciphertext.data(), ciphertext.size())) {
        EVP_CIPHER_CTX_free(ctx);
        handleErrors();
    }

    int finalLen;
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + plaintextLen, &finalLen)) {
        EVP_CIPHER_CTX_free(ctx);
        handleErrors();
    }

    plaintextLen += finalLen;
    plaintext.resize(plaintextLen);

    writeFile(outputFilePath, plaintext);

    EVP_CIPHER_CTX_free(ctx);
}
#ifndef SHA512_HASHER_H
#define SHA512_HASHER_H

#include <string>
#include "Cipher.h"  

class SHA512Hasher : public Cipher {
private:
    std::string salt;  

public:
    SHA512Hasher() = default;
    explicit SHA512Hasher(const std::string& salt) : salt(salt) {}

    // Для хеширования файлов
    void encryptFile(const std::string& inputFilePath, const std::string& outputFilePath) override;
    void decryptFile(const std::string&, const std::string&) override {
        // SHA-512 не поддерживает дешифрование - это хеш-функция
        throw std::runtime_error("Decryption not supported for SHA-512(Хеш это одностороная функция)");
    }

    // Дополнительные методы для строк
    static std::string hashString(const std::string& input, const std::string& salt = "");
};

#endif // SHA512_HASHER_H
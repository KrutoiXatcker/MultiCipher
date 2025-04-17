#ifndef AES_CIPHER_H
#define AES_CIPHER_H


#include <vector>
#include <string>
#include <stdexcept>
#include <openssl/aes.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include "Cipher.h"


class AESCipher : public Cipher {
public:
    explicit AESCipher(const std::string& key);
    explicit AESCipher(const std::vector<unsigned char>& key);
    ~AESCipher() override = default;

    void encryptFile(const std::string& inputFilePath, const std::string& outputFilePath) override;
    void decryptFile(const std::string& inputFilePath, const std::string& outputFilePath) override;

private:
    std::vector<unsigned char> key;
    std::vector<unsigned char> iv;

    void handleErrors();
    void generateIV();
    size_t readFile(const std::string& filePath, std::vector<unsigned char>& buffer);
    void writeFile(const std::string& filePath, const std::vector<unsigned char>& buffer);
};

#endif // AES_CIPHER_H
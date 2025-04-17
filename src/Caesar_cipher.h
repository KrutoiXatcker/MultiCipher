#ifndef CAESAR_CIPHER_H
#define CAESAR_CIPHER_H

#include <string>
#include "Cipher.h"

class Caesar_cipher : public Cipher {
private:

public:
    short password = 3; // Значение сдвига для шифра

    void encryptFile(const std::string& inputFilePath, const std::string& outputFilePath) override;
    void decryptFile(const std::string& inputFilePath, const std::string& outputFilePath) override;
};

#endif // CAESAR_CIPHER_H


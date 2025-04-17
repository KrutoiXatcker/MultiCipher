#ifndef CIPHER_H
#define CIPHER_H


#include <string>
#include <fstream>
#include <vector>
#include <stdexcept>


class Cipher {
public:
    virtual ~Cipher() = default;
    
    /**
     * Шифрует файл
     * @param inputFilePath Путь к исходному файлу
     * @param outputFilePath Путь для сохранения зашифрованного файла
     * @throws std::runtime_error Если не удалось прочитать/записать файл
     */
    virtual void encryptFile(const std::string& inputFilePath, const std::string& outputFilePath) = 0;
    
    /**
     * Дешифрует файл
     * @param inputFilePath Путь к зашифрованному файлу
     * @param outputFilePath Путь для сохранения расшифрованного файла
     * @throws std::runtime_error Если не удалось прочитать/записать файл
     */
    virtual void decryptFile(const std::string& inputFilePath, const std::string& outputFilePath) = 0;
    

};

#endif
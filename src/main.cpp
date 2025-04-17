#include <iostream>
#include <getopt.h>
#include <stdexcept>


#include "Cipher.h"
#include "Caesar_cipher.h"  
#include "SHA512Hasher.h"
#include "AES.h"

void encryptFile(Cipher& cipher, const std::string& inputFilePath, const std::string& outputFilePath) {
    cipher.encryptFile(inputFilePath, outputFilePath);
}

void decryptFile(Cipher& cipher, const std::string& inputFilePath, const std::string& outputFilePath) {
    cipher.decryptFile(inputFilePath, outputFilePath);
}

void processFile(Cipher& cipher, const std::string& inputFile, 
    const std::string& outputFile, bool encrypt) {
    if (encrypt) {
        cipher.encryptFile(inputFile, outputFile);
    } else {
        cipher.decryptFile(inputFile, outputFile);
    }
}

// Объявление функции, которая печатает help
void printHelp(const std::string& programName) {
    std::cout << "Usage: " << programName << " [options]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -i, --input <file>        Input file to process." << std::endl;
    std::cout << "  -o, --output <file>       Output file. Defaults to 'cript.out'." << std::endl;
    std::cout << "  -P, --password <string>   Password/key to use for encryption/decryption." << std::endl;
    std::cout << "  -c, --cipher <type>       Cipher type to use: " << std::endl;
    std::cout << "                            1: Caesar cipher" << std::endl;
    std::cout << "                            2: AES (Размер ключа для AES-256 (в байтах))" << std::endl;
    std::cout << "                            3: SHA-512" << std::endl;
    std::cout << "  -O, --opereting <mode>     Operation mode:" << std::endl;
    std::cout << "                            0: Decrypt" << std::endl;
    std::cout << "                            1: Encrypt" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  " << programName << " -c 1 -O 1 -i input.txt -o output.enc  (Encrypt input.txt with Caesar cipher)" << std::endl;
    std::cout << "  " << programName << " -c 2 -O 0 -i encrypted.enc -o decrypted.txt -P mysecretkey (Decrypt with AES)" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char** argv) {

    if (argc == 1) {
        printHelp(argv[0]);
        return 1; // Или 0, если вы считаете, что отсутствие аргументов - это нормальное поведение
    }

    Cipher* buf = nullptr;
    std::string input_file;
    std::string output_file = "cript.out";
    std::string password_path;
    std::string password;
    std::string cipher_type;
    std::string opereting = "0";

    static struct option long_options[] = {
        {"input", required_argument, 0, 'i'},
        {"output", required_argument, 0, 'o'},
        {"password", required_argument, 0, 'P'},
        {"password_path", required_argument, 0, 'p'},
        {"cipher", required_argument, 0, 'c'},
        {"opereting", required_argument, 0, 'O'},
        {0, 0, 0, 0}
    };

    int option;
    int option_index = 0;

    while ((option = getopt_long(argc, argv, "i:o:P:p:c:O:", long_options, &option_index)) != -1) {
        if (option == -1) break;
        
        switch (option) {
            case 'i':
                input_file = optarg;
                std::cout << "input_file = " << input_file << std::endl;
                break;
            case 'o':
                output_file = optarg;
                std::cout << "output_file = " << output_file << std::endl;
                break;
            case 'P':
                password = optarg;
                std::cout << "password = " << password << std::endl;
                break;
            case 'p':
                password_path = optarg;
                std::cout << "password_path = " << password_path << std::endl;
                break;
            case 'c':
                cipher_type = optarg;
                std::cout << "cipher_type = " << cipher_type << std::endl;
                break;
            case 'O':
                opereting = optarg;
                std::cout << "Operation: " << opereting << std::endl;
                break;
            case '?':
                std::cout << "Error: Unknown option" << std::endl;
                return 1;
            default:
                break;
        }
    }

    try {
        if (cipher_type.empty()) {
            throw std::invalid_argument("Cipher type not specified");
        }

        int cipher_num = std::stoi(cipher_type);
        
        switch(cipher_num) {
            case 1:
            {
                buf = new Caesar_cipher();
                if (!password.empty()) {
                    static_cast<Caesar_cipher*>(buf)->password = std::stoi(password);
                }
                break;
            }
            case 2:  // Добавляем обработку для AES
            {
                // Для AES пароль будет использоваться как ключ
                if (password.empty()) {
                    throw std::invalid_argument("Password is required for AES cipher");
                }

                // Создаем AES шифр, передавая пароль напрямую
                buf = new AESCipher(password);
                break;
            }
            case 3:
                buf = new SHA512Hasher(password);
                break;
            default:
                throw std::invalid_argument("Unsupported cipher type");
        }

        if (input_file.empty()) {
            throw std::invalid_argument("Input file not specified");
        }

        int operet = std::stoi(opereting);

        // Выполнение операции
        processFile(*buf, input_file, output_file , std::stoi(opereting));
    } 
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        if (buf) delete buf;
        return 1;
    }

    if (buf) delete buf;
    return 0;
}
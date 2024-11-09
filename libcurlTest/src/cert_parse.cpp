#include <openssl/bio.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

std::string readFile(const std::filesystem::path &filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath.string());
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int parse_pem_certificate()
{
    const std::filesystem::path certFilePath = R"(D:\workspace\cprojects\libcurlTest\sm2-root.crt)"; // 替换为你的证书文件路径

    std::string cert_pem;
    try {
        cert_pem = readFile(certFilePath);
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    BIO *bio = BIO_new_mem_buf(cert_pem.c_str(), -1);
    if (!bio) {
        std::cerr << "Failed to create BIO\n";
        return 1;
    }

    X509 *cert = PEM_read_bio_X509(bio, nullptr, nullptr, nullptr);
    if (!cert) {
        std::cerr << "Failed to parse certificate\n";
        BIO_free(bio);
        return 1;
    }


    ASN1_INTEGER *serial = X509_get_serialNumber(cert);
    if (!serial) {
        std::cerr << "Failed to get serial number\n";
        X509_free(cert);
        BIO_free(bio);
        return 1;
    }

    BIGNUM *bn = ASN1_INTEGER_to_BN(serial, nullptr);
    if (!bn) {
        std::cerr << "Failed to convert serial number to BIGNUM\n";
        X509_free(cert);
        BIO_free(bio);
        return 1;
    }

    char *serial_str = BN_bn2hex(bn);
    if (!serial_str) {
        std::cerr << "Failed to convert BIGNUM to string\n";
        BN_free(bn);
        X509_free(cert);
        BIO_free(bio);
        return 1;
    }

    std::cout << "Certificate Serial Number: " << serial_str << "\n";

    OPENSSL_free(serial_str);
    BN_free(bn);
    X509_free(cert);
    BIO_free(bio);

    return 0;
}

int parse_der_cert()
{
    const std::filesystem::path certFilePath = R"(D:\workspace\cprojects\libcurlTest\sm2-root.der)"; // 替换为你的证书文件路径

    std::string cert_der;
    try {
        cert_der = readFile(certFilePath);
    } catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
        return 1;
    }

    BIO *bio = BIO_new_mem_buf(cert_der.data(), cert_der.size());
    if (!bio) {
        std::cerr << "Failed to create BIO\n";
        return 1;
    }

    X509 *cert = d2i_X509_bio(bio, nullptr);
    if (!cert) {
        std::cerr << "Failed to parse certificate\n";
        BIO_free(bio);
        return 1;
    }

    ASN1_INTEGER *serial = X509_get_serialNumber(cert);
    if (!serial) {
        std::cerr << "Failed to get serial number\n";
        X509_free(cert);
        BIO_free(bio);
        return 1;
    }

    BIGNUM *bn = ASN1_INTEGER_to_BN(serial, nullptr);
    if (!bn) {
        std::cerr << "Failed to convert serial number to BIGNUM\n";
        X509_free(cert);
        BIO_free(bio);
        return 1;
    }

    char *serial_str = BN_bn2hex(bn);
    if (!serial_str) {
        std::cerr << "Failed to convert BIGNUM to string\n";
        BN_free(bn);
        X509_free(cert);
        BIO_free(bio);
        return 1;
    }

    std::cout << "Certificate Serial Number: " << serial_str << "\n";

    OPENSSL_free(serial_str);
    BN_free(bn);
    X509_free(cert);
    BIO_free(bio);

    return 0;

}

int main() 
{
    parse_der_cert();
}
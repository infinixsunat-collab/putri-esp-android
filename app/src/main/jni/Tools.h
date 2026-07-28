#include "Includes.h"

#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <openssl/md5.h>
#include <openssl/hmac.h>

namespace Tools
{
    std::string CalcMD5(std::string s);
    std::string CalcSHA256(std::string s);
    std::string CalcHMAC(const std::string& key, const std::string& data);
    std::string DecryptString(const unsigned char* data, size_t len, unsigned char key);
    uint32_t CalcCRC32(const unsigned char* data, size_t len);
    uint64_t CalcXOR64(const unsigned char* data, size_t len);
}
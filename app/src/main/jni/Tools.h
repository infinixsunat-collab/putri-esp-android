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
}
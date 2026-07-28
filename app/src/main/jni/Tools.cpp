#include "Tools.h"

std::string Tools::CalcMD5(std::string s) {
    std::string result;

    unsigned char hash[MD5_DIGEST_LENGTH];
    char tmp[4];

    MD5_CTX md5;
    MD5_Init(&md5);
    MD5_Update(&md5, s.c_str(), s.length());
    MD5_Final(hash, &md5);
    for (unsigned char i : hash) {
        sprintf(tmp, "%02x", i);
        result += tmp;
    }
    return result;
}

std::string Tools::CalcHMAC(const std::string& key, const std::string& data) {
    unsigned char result[MD5_DIGEST_LENGTH];
    unsigned int result_len = 0;

    HMAC(EVP_md5(),
         key.c_str(), (int)key.length(),
         (const unsigned char*)data.c_str(), data.length(),
         result, &result_len);

    char tmp[4];
    std::string out;
    for (unsigned int i = 0; i < result_len; i++) {
        sprintf(tmp, "%02x", result[i]);
        out += tmp;
    }
    return out;
}

std::string Tools::DecryptString(const unsigned char* data, size_t len, unsigned char key) {
    std::string result;
    result.reserve(len);
    for (size_t i = 0; i < len; i++) {
        result += (char)(data[i] ^ key);
    }
    return result;
}

uint32_t Tools::CalcCRC32(const unsigned char* data, size_t len) {
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (int j = 0; j < 8; j++) {
            if (crc & 1)
                crc = (crc >> 1) ^ 0xEDB88320;
            else
                crc >>= 1;
        }
    }
    return ~crc;
}

uint64_t Tools::CalcXOR64(const unsigned char* data, size_t len) {
    uint64_t result = 0;
    size_t i = 0;
    for (; i + 8 <= len; i += 8) {
        uint64_t val;
        memcpy(&val, data + i, 8);
        result ^= val;
    }
    if (i < len) {
        uint64_t val = 0;
        memcpy(&val, data + i, len - i);
        result ^= val;
    }
    return result;
}

std::string Tools::CalcSHA256(std::string s) {
    std::string result;

    unsigned char hash[SHA256_DIGEST_LENGTH];
    char tmp[4];

    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, s.c_str(), s.length());
    SHA256_Final(hash, &sha256);
    for (unsigned char i : hash) {
        sprintf(tmp, "%02x", i);
        result += tmp;
    }
    return result;
}
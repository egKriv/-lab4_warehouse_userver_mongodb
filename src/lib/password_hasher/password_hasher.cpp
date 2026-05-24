#include "password_hasher.hpp"
#include <openssl/sha.h>

namespace lab2::infrastructure {
    std::string PasswordHasher::Hash(const std::string& password) const {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256(reinterpret_cast<const unsigned char*>(password.c_str()), password.size(), hash);
        std::string hex;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
            char buf[3];
            snprintf(buf, sizeof(buf), "%02x", hash[i]);
            hex += buf;
        }
        return hex;
    }
}
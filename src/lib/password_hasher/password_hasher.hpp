#pragma once
#include <string>

namespace lab2::infrastructure {
    class PasswordHasher {
    public:
        std::string Hash(const std::string& password) const;
    };
}
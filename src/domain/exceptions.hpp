#pragma once
#include <string>
#include <stdexcept>

namespace inventory::exceptions::domain {
    struct ValidationError : std::runtime_error {
        std::string field;
        std::string message;
        ValidationError(const std::string& f, const std::string& m)
            : std::runtime_error(m), field(f), message(m) {}
    };
}
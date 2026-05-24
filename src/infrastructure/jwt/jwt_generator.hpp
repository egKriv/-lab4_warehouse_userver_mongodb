#pragma once

#include <string>
#include <boost/uuid/uuid.hpp>

namespace inventory::infrastructure {

class JwtGenerator {
public:
    JwtGenerator(const std::string& secret, const std::string& issuer, const std::string& audience);

    std::string GenerateToken(const boost::uuids::uuid& user_id, const std::string& login,
                              const std::string& role) const;

private:
    std::string secret_;
    std::string issuer_;
    std::string audience_;
};

} // namespace inventory::infrastructure
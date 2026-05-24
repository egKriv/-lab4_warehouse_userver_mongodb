#include "jwt_generator.hpp"
#include <jwt-cpp/jwt.h>
#include <boost/uuid/uuid_io.hpp>

namespace inventory::infrastructure {

JwtGenerator::JwtGenerator(const std::string& secret, const std::string& issuer, const std::string& audience)
    : secret_(secret), issuer_(issuer), audience_(audience) {}

std::string JwtGenerator::GenerateToken(const boost::uuids::uuid& user_id, const std::string& login,
                                        const std::string& role) const {
    auto now = std::chrono::system_clock::now();
    return jwt::create()
        .set_issuer(issuer_)
        .set_audience(audience_)
        .set_issued_at(now)
        .set_expires_at(now + std::chrono::hours{24})
        .set_payload_claim("sub", jwt::claim(boost::uuids::to_string(user_id)))
        .set_payload_claim("login", jwt::claim(login))
        .set_payload_claim("role", jwt::claim(role))
        .sign(jwt::algorithm::hs256{secret_});
}

} // namespace inventory::infrastructure
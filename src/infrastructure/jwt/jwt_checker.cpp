#include "jwt_checker.hpp"
#include <jwt-cpp/jwt.h>

namespace inventory::infrastructure {

JwtChecker::JwtChecker(const std::string& secret, const std::string& issuer, const std::string& audience)
    : secret_(secret), issuer_(issuer), audience_(audience) {}

userver::server::handlers::auth::AuthCheckResult JwtChecker::CheckAuth(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& request_context) const {
    const auto& auth_header = request.GetHeader("Authorization");
    if (auth_header.empty()) {
        return userver::server::handlers::auth::AuthCheckResult{
            userver::server::handlers::auth::AuthCheckResult::Status::kTokenNotFound,
            "Missing 'Authorization' header"
        };
    }

    const std::string bearer_prefix = "Bearer ";
    if (auth_header.size() < bearer_prefix.size() ||
        auth_header.substr(0, bearer_prefix.size()) != bearer_prefix) {
        return userver::server::handlers::auth::AuthCheckResult{
            userver::server::handlers::auth::AuthCheckResult::Status::kTokenNotFound,
            "'Authorization' header should have 'Bearer some-token' format"
        };
    }

    std::string token = auth_header.substr(bearer_prefix.size());
    auto user_data = DecodeAndVerify(token);
    if (!user_data) {
        return userver::server::handlers::auth::AuthCheckResult{
            userver::server::handlers::auth::AuthCheckResult::Status::kInvalidToken,
            "Invalid or expired token"
        };
    }

    request_context.SetData("user_id", user_data->user_id);
    request_context.SetData("login", user_data->login);
    request_context.SetData("role", user_data->role);

    return {};
}

std::optional<JwtUserData> JwtChecker::DecodeAndVerify(const std::string& token) const {
    try {
        auto decoded = jwt::decode(token);
        jwt::verify()
            .allow_algorithm(jwt::algorithm::hs256{secret_})
            .with_issuer(issuer_)
            .with_audience(audience_)
            .verify(decoded);

        JwtUserData data;
        data.user_id = decoded.get_payload_claim("sub").as_string();
        data.login = decoded.get_payload_claim("login").as_string();
        data.role = decoded.get_payload_claim("role").as_string();
        return data;
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

} // namespace inventory::infrastructure
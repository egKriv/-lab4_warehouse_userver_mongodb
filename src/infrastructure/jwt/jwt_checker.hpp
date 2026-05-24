#pragma once

#include <userver/server/handlers/auth/auth_checker_base.hpp>
#include <userver/server/http/http_request.hpp>
#include <string>
#include <optional>

namespace inventory::infrastructure {

struct JwtUserData {
    std::string user_id;
    std::string login;
    std::string role;
};

class JwtChecker final : public userver::server::handlers::auth::AuthCheckerBase {
public:
    JwtChecker(const std::string& secret, const std::string& issuer, const std::string& audience);

    userver::server::handlers::auth::AuthCheckResult CheckAuth(
        const userver::server::http::HttpRequest& request,
        userver::server::request::RequestContext& request_context) const override;

    bool SupportsUserAuth() const noexcept override { return false; }

private:
    std::optional<JwtUserData> DecodeAndVerify(const std::string& token) const;
    std::string secret_;
    std::string issuer_;
    std::string audience_;
};

} // namespace inventory::infrastructure
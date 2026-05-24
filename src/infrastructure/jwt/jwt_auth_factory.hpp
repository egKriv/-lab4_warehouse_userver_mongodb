#pragma once

#include <userver/server/handlers/auth/auth_checker_factory.hpp>
#include <userver/components/component_context.hpp>
#include "jwt_component.hpp"

namespace inventory::infrastructure {

class JwtAuthCheckerFactory final : public userver::server::handlers::auth::AuthCheckerFactoryBase {
public:
    static constexpr std::string_view kAuthType = "jwt-auth";

    explicit JwtAuthCheckerFactory(const userver::components::ComponentContext& context)
        : auth_(context.FindComponent<JwtAuthComponent>()) {}

    userver::server::handlers::auth::AuthCheckerBasePtr MakeAuthChecker(
        const userver::server::handlers::auth::HandlerAuthConfig&) const override {
        return auth_.GetChecker();
    }

private:
    JwtAuthComponent& auth_;
};

} // namespace inventory::infrastructure
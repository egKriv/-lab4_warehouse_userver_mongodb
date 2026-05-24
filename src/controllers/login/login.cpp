#include "login.hpp"
#include <domain/user.hpp>
#include <lib/password_hasher/password_hasher.hpp>
#include <userver/server/handlers/exceptions.hpp>
#include <infrastructure/jwt/jwt_generator.hpp>

namespace inventory::controllers {

Login::Login(const userver::components::ComponentConfig& config,
             const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      storage_(context.FindComponent<inventory::storage::PostgresStorage>()),
      jwt_gen_(context.FindComponent<inventory::infrastructure::JwtAuthComponent>().GetGenerator()) {}

userver::formats::json::Value Login::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& /*request*/,
    const userver::formats::json::Value& json,
    userver::server::request::RequestContext& /*context*/) const {
    auto login = json["login"].As<std::string>();
    auto password = json["password"].As<std::string>();

    auto user = storage_.GetUserByLogin(login);
    if (!user) {
        throw userver::server::handlers::Unauthorized(userver::server::handlers::ExternalBody{
            R"({"code":"unauthorized","message":"Invalid credentials"})"});
    }

    lab2::infrastructure::PasswordHasher hasher;
    if (hasher.Hash(password) != user->GetPasswordHash()) {
        throw userver::server::handlers::Unauthorized(userver::server::handlers::ExternalBody{
            R"({"code":"unauthorized","message":"Invalid credentials"})"});
    }

    auto token = jwt_gen_->GenerateToken(user->GetId(), user->GetLogin(), user->GetRole());
    userver::formats::json::ValueBuilder resp;
    resp["token"] = token;
    return resp.ExtractValue();
}

} // namespace inventory::controllers
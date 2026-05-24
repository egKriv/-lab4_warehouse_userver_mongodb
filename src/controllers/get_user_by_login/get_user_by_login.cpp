#include "get_user_by_login.hpp"
#include <userver/server/handlers/exceptions.hpp>
#include <lib/json_builders/json_builders.hpp>

namespace inventory::controllers {

GetUserByLogin::GetUserByLogin(const userver::components::ComponentConfig& config,
                               const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      user_service_(context.FindComponent<inventory::storage::PostgresStorage>()) {}

userver::formats::json::Value GetUserByLogin::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& /*json*/,
    userver::server::request::RequestContext& /*context*/) const {
    const auto login = request.GetPathArg("login");
    auto user = user_service_.GetByLogin(login);
    if (!user) {
        request.SetResponseStatus(userver::server::http::HttpStatus::kNotFound);
        return userver::formats::json::FromString(R"({"code":"not_found","message":"User not found"})");
    }
    return *user;
}

} // namespace inventory::controllers
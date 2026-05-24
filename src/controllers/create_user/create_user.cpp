#include "create_user.hpp"
#include <domain/user.hpp>
#include <userver/components/component_context.hpp>
#include <userver/server/handlers/exceptions.hpp>

namespace inventory::controllers {

CreateUser::CreateUser(const userver::components::ComponentConfig& config,
                       const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      user_service_(context.FindComponent<inventory::storage::PostgresStorage>()) {}

userver::formats::json::Value CreateUser::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& json,
    userver::server::request::RequestContext& /*context*/) const {
    auto result = domain::User::Create(
        json["login"].As<std::string>(),
        json["password"].As<std::string>(),
        json["first_name"].As<std::string>(),
        json["last_name"].As<std::string>(),
        json["email"].As<std::string>(),
        json.HasMember("role") ? json["role"].As<std::string>() : "storekeeper"
    );
    if (std::holds_alternative<exceptions::domain::ValidationError>(result)) {
        auto& err = std::get<exceptions::domain::ValidationError>(result);
        throw userver::server::handlers::ClientError(userver::server::handlers::ExternalBody{
            fmt::format("{{\"code\":\"validation_error\",\"message\":\"{}\"}}", err.what())});
    }
    auto user = std::get<domain::User>(result);
    auto json_resp = user_service_.Register(user);
    if (!json_resp) {
        throw userver::server::handlers::ConflictError(userver::server::handlers::ExternalBody{
            R"({"code":"conflict","message":"User already exists"})"});
    }
    request.SetResponseStatus(userver::server::http::HttpStatus::kCreated);
    return *json_resp;
}

} // namespace inventory::controllers
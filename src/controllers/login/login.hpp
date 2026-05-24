#pragma once

#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/components/component_context.hpp>
#include <infrastructure/jwt/jwt_component.hpp>
#include <infrastructure/postgres_storage/postgres_storage.hpp>

namespace inventory::controllers {

class Login final : public userver::server::handlers::HttpHandlerJsonBase {
public:
    static constexpr std::string_view kName = "HandlerLogin";
    Login(const userver::components::ComponentConfig& config,
          const userver::components::ComponentContext& context);

    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest& request,
        const userver::formats::json::Value& json,
        userver::server::request::RequestContext& context) const override;

private:
    inventory::storage::PostgresStorage& storage_;
    std::shared_ptr<inventory::infrastructure::JwtGenerator> jwt_gen_;
};

} // namespace inventory::controllers
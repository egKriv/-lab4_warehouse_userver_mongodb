#include "add_product.hpp"
#include <domain/product.hpp>
#include <userver/server/handlers/exceptions.hpp>

namespace inventory::controllers {

AddProduct::AddProduct(const userver::components::ComponentConfig& config,
                       const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      product_service_(context.FindComponent<inventory::storage::PostgresStorage>()) {}

userver::formats::json::Value AddProduct::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& json,
    userver::server::request::RequestContext& /*context*/) const {
    auto result = domain::Product::Create(
        json["name"].As<std::string>(),
        json.HasMember("description") ? json["description"].As<std::string>() : "",
        json.HasMember("sku") ? json["sku"].As<std::string>() : "",
        json["initial_quantity"].As<int>(),
        json.HasMember("unit") ? json["unit"].As<std::string>() : "pcs"
    );
    if (std::holds_alternative<exceptions::domain::ValidationError>(result)) {
        auto& err = std::get<exceptions::domain::ValidationError>(result);
        throw userver::server::handlers::ClientError(userver::server::handlers::ExternalBody{
            fmt::format("{{\"code\":\"validation_error\",\"message\":\"{}\"}}", err.what())});
    }
    auto product = std::get<domain::Product>(result);
    auto json_resp = product_service_.Add(product);
    if (!json_resp) {
        throw userver::server::handlers::InternalServerError(userver::server::handlers::ExternalBody{
            R"({"code":"internal_error","message":"Failed to add product"})"});
    }
    request.SetResponseStatus(userver::server::http::HttpStatus::kCreated);
    return *json_resp;
}

} // namespace inventory::controllers
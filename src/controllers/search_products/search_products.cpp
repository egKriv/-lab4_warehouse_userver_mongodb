#include "search_products.hpp"
#include <userver/server/handlers/exceptions.hpp>
#include <userver/components/component_context.hpp>
#include <services/product_service/product_service.hpp>
#include <lib/json_builders/json_builders.hpp>

namespace inventory::controllers {

SearchProducts::SearchProducts(const userver::components::ComponentConfig& config,
                               const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      product_service_(context.FindComponent<inventory::storage::PostgresStorage>()) {}

userver::formats::json::Value SearchProducts::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& /*json*/,
    userver::server::request::RequestContext& /*context*/) const {
    const auto& name = request.GetArg("name");
    if (name.empty()) {
        throw userver::server::handlers::ClientError(userver::server::handlers::ExternalBody{
            R"({"code":"validation_error","message":"name query parameter is required"})"});
    }
    auto products = product_service_.SearchByName(name);
    userver::formats::json::ValueBuilder builder;
    userver::formats::json::ValueBuilder items;
    for (const auto& p : products) {
        items.PushBack(p);
    }
    builder["items"] = std::move(items);
    builder["total"] = static_cast<int>(products.size());
    return builder.ExtractValue();
}

} // namespace inventory::controllers
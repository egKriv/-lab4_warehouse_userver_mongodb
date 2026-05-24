#include "get_stock.hpp"
#include <userver/server/handlers/exceptions.hpp>

namespace inventory::controllers {

GetStock::GetStock(const userver::components::ComponentConfig& config,
                   const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      product_service_(context.FindComponent<inventory::storage::PostgresStorage>()) {}

userver::formats::json::Value GetStock::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& /*json*/,
    userver::server::request::RequestContext& /*context*/) const {
    int limit = 20, offset = 0;
    try {
        if (request.HasArg("limit")) limit = std::stoi(request.GetArg("limit"));
        if (request.HasArg("offset")) offset = std::stoi(request.GetArg("offset"));
    } catch (...) {
        throw userver::server::handlers::ClientError(userver::server::handlers::ExternalBody{
            R"({"code":"validation_error","message":"Invalid limit or offset"})"});
    }
    return product_service_.GetStock(limit, offset);
}

} // namespace inventory::controllers
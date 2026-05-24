#include "get_receipts.hpp"
#include <userver/server/handlers/exceptions.hpp>

namespace inventory::controllers {

GetReceipts::GetReceipts(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      receipt_service_(context.FindComponent<inventory::storage::PostgresStorage>()) {}

userver::formats::json::Value GetReceipts::HandleRequestJsonThrow(
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
    if (request.HasArg("product_id") && !request.GetArg("product_id").empty()) {
        return receipt_service_.GetByProductId(request.GetArg("product_id"), limit, offset);
    }
    return receipt_service_.GetAll(limit, offset);
}

} // namespace inventory::controllers
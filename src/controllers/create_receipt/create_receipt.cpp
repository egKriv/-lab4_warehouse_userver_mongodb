#include "create_receipt.hpp"
#include <domain/receipt.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <userver/server/handlers/exceptions.hpp>

namespace inventory::controllers {

CreateReceipt::CreateReceipt(const userver::components::ComponentConfig& config,
                             const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      receipt_service_(context.FindComponent<inventory::storage::PostgresStorage>()) {}

userver::formats::json::Value CreateReceipt::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& json,
    userver::server::request::RequestContext& /*context*/) const {
    boost::uuids::uuid product_id;
    try {
        product_id = boost::lexical_cast<boost::uuids::uuid>(json["product_id"].As<std::string>());
    } catch (...) {
        throw userver::server::handlers::ClientError(userver::server::handlers::ExternalBody{
            R"({"code":"validation_error","message":"Invalid product_id UUID"})"});
    }
    int quantity = json["quantity"].As<int>();
    std::string supplier = json["supplier"].As<std::string>();
    std::string created_by = "system"; // в реальном коде брать из контекста JWT

    auto receipt = domain::Receipt::Create(product_id, quantity, supplier, created_by);
    auto json_resp = receipt_service_.Create(receipt);
    if (!json_resp) {
        throw userver::server::handlers::InternalServerError(userver::server::handlers::ExternalBody{
            R"({"code":"internal_error","message":"Failed to create receipt"})"});
    }
    request.SetResponseStatus(userver::server::http::HttpStatus::kCreated);
    return *json_resp;
}

} // namespace inventory::controllers
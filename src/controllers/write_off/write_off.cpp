
#include "write_off.hpp"
#include <userver/server/handlers/exceptions.hpp>
#include <userver/components/component_context.hpp>
#include <userver/storages/mongo/component.hpp>
#include <userver/formats/bson/value_builder.hpp>
#include <chrono>
#include <iomanip>

namespace inventory::controllers {

WriteOff::WriteOff(const userver::components::ComponentConfig& config,
                   const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      product_service_(context.FindComponent<inventory::storage::PostgresStorage>()),
      mongo_pool_(context.FindComponent<userver::components::Mongo>("mongo-db").GetPool()) {}

userver::formats::json::Value WriteOff::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& /*request*/,
    const userver::formats::json::Value& json,
    userver::server::request::RequestContext& /*context*/) const {

    auto product_id = json["product_id"].As<std::string>();
    int quantity = json["quantity"].As<int>();
    std::string reason = json.HasMember("reason") ? json["reason"].As<std::string>() : "unspecified";

    bool ok = product_service_.WriteOff(product_id, quantity);
    if (!ok) {
        throw userver::server::handlers::ClientError(userver::server::handlers::ExternalBody{
            R"({"code":"conflict","message":"Insufficient stock or product not found"})"});
    }

    // Запись события списания в MongoDB
    auto now = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::gmtime(&tt);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ");

    userver::formats::bson::ValueBuilder doc;
    doc["product_id"] = product_id;
    doc["quantity"] = quantity;
    doc["reason"] = reason;
    doc["created_at"] = oss.str();

    auto collection = mongo_pool_->GetCollection("writeoffs");
    collection.InsertOne(doc.ExtractValue());

    userver::formats::json::ValueBuilder resp;
    resp["message"] = "Product written off successfully";
    resp["product_id"] = product_id;
    resp["remaining_quantity"] = 0;
    return resp.ExtractValue();
}

} // namespace inventory::controllers
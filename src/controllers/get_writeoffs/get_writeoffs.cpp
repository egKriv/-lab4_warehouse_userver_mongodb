#include "get_writeoffs.hpp"
#include <userver/components/component_context.hpp>
#include <userver/storages/mongo/component.hpp>
#include <userver/formats/bson/value_builder.hpp>
#include <userver/server/handlers/exceptions.hpp>

namespace inventory::controllers {

GetWriteoffs::GetWriteoffs(const userver::components::ComponentConfig& config,
                           const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      mongo_pool_(context.FindComponent<userver::components::Mongo>("mongo-db").GetPool()) {}

userver::formats::json::Value GetWriteoffs::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& /*json*/,
    userver::server::request::RequestContext& /*context*/) const {

    auto collection = mongo_pool_->GetCollection("writeoffs");

    // Опциональный фильтр по product_id
    userver::formats::bson::ValueBuilder filter_builder;
    if (request.HasArg("product_id") && !request.GetArg("product_id").empty()) {
        filter_builder["product_id"] = request.GetArg("product_id");
    }
    // Если фильтр не задан, filter_builder остаётся пустым документом {}

    auto cursor = collection.Find(filter_builder.ExtractValue());

    userver::formats::json::ValueBuilder items;
    for (const auto& doc : cursor) {
        userver::formats::json::ValueBuilder item;
        item["product_id"] = doc["product_id"].As<std::string>();
        item["quantity"] = doc["quantity"].As<int>();
        item["reason"] = doc["reason"].As<std::string>("");
        item["created_at"] = doc["created_at"].As<std::string>();
        items.PushBack(std::move(item));
    }

    return items.ExtractValue();
}

} // namespace inventory::controllers
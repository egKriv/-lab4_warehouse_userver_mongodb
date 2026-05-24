#include "product_service.hpp"

namespace inventory::services {

std::optional<userver::formats::json::Value> ProductService::Add(const domain::Product& product) {
    auto stored = storage_.CreateProduct(product);
    if (!stored) return std::nullopt;
    return json_builders::BuildProductJson(*stored);
}

std::vector<userver::formats::json::Value> ProductService::SearchByName(const std::string& name) {
    auto products = storage_.SearchProductsByName(name);
    std::vector<userver::formats::json::Value> result;
    for (const auto& p : products) {
        result.push_back(json_builders::BuildProductJson(p));
    }
    return result;
}

userver::formats::json::Value ProductService::GetStock(int limit, int offset) {
    auto page = storage_.GetAllProducts(limit, offset);
    userver::formats::json::ValueBuilder builder;
    builder["total"] = page.total;
    userver::formats::json::ValueBuilder items;
    for (const auto& p : page.items) {
        items.PushBack(json_builders::BuildProductJson(p));
    }
    builder["items"] = std::move(items);
    return builder.ExtractValue();
}

bool ProductService::WriteOff(const std::string& product_id, int quantity) {
    return storage_.UpdateProductQuantity(product_id, -quantity);
}

}
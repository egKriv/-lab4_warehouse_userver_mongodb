#include "receipt_service.hpp"
#include <boost/uuid/uuid_io.hpp>

namespace inventory::services {

std::optional<userver::formats::json::Value> ReceiptService::Create(const domain::Receipt& receipt) {
    auto stored = storage_.CreateReceipt(receipt);
    if (!stored) return std::nullopt;
    storage_.UpdateProductQuantity(boost::uuids::to_string(receipt.GetProductId()), receipt.GetQuantity());
    return json_builders::BuildReceiptJson(*stored);
}

userver::formats::json::Value ReceiptService::GetByProductId(const std::string& product_id, int limit, int offset) {
    auto page = storage_.GetReceiptsByProductId(product_id, limit, offset);
    userver::formats::json::ValueBuilder builder;
    builder["total"] = page.total;
    userver::formats::json::ValueBuilder items;
    for (const auto& r : page.items) {
        items.PushBack(json_builders::BuildReceiptJson(r));
    }
    builder["items"] = std::move(items);
    return builder.ExtractValue();
}

userver::formats::json::Value ReceiptService::GetAll(int limit, int offset) {
    auto page = storage_.GetAllReceipts(limit, offset);
    userver::formats::json::ValueBuilder builder;
    builder["total"] = page.total;
    userver::formats::json::ValueBuilder items;
    for (const auto& r : page.items) {
        items.PushBack(json_builders::BuildReceiptJson(r));
    }
    builder["items"] = std::move(items);
    return builder.ExtractValue();
}

} // namespace inventory::services
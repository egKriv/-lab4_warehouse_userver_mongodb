#include "json_builders.hpp"
#include <boost/uuid/uuid_io.hpp>
#include <chrono>

namespace inventory::json_builders {

using namespace userver::formats::json;

Value BuildUserJson(const domain::User& user) {
    ValueBuilder builder(userver::formats::common::Type::kObject);
    builder["id"] = boost::uuids::to_string(user.GetId());
    builder["login"] = user.GetLogin();
    builder["first_name"] = user.GetFirstName();
    builder["last_name"] = user.GetLastName();
    builder["email"] = user.GetEmail();
    builder["role"] = user.GetRole();
    builder["created_at"] = std::chrono::system_clock::to_time_t(user.GetCreatedAt());
    return builder.ExtractValue();
}

Value BuildProductJson(const domain::Product& product) {
    ValueBuilder builder(userver::formats::common::Type::kObject);
    builder["id"] = boost::uuids::to_string(product.GetId());
    builder["name"] = product.GetName();
    builder["description"] = product.GetDescription();
    builder["sku"] = product.GetSku();
    builder["quantity_on_hand"] = product.GetQuantityOnHand();
    builder["unit"] = product.GetUnit();
    return builder.ExtractValue();
}

Value BuildReceiptJson(const domain::Receipt& receipt) {
    ValueBuilder builder(userver::formats::common::Type::kObject);
    builder["id"] = boost::uuids::to_string(receipt.GetId());
    builder["product_id"] = boost::uuids::to_string(receipt.GetProductId());
    builder["quantity"] = receipt.GetQuantity();
    builder["supplier"] = receipt.GetSupplier();
    builder["receipt_date"] = std::chrono::system_clock::to_time_t(receipt.GetReceiptDate());
    builder["created_by"] = receipt.GetCreatedBy();
    return builder.ExtractValue();
}

} // namespace inventory::json_builders
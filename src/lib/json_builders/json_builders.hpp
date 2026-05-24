#pragma once
#include <userver/formats/json/value_builder.hpp>
#include <domain/user.hpp>
#include <domain/product.hpp>
#include <domain/receipt.hpp>

namespace inventory::json_builders {

userver::formats::json::Value BuildUserJson(const domain::User& user);
userver::formats::json::Value BuildProductJson(const domain::Product& product);
userver::formats::json::Value BuildReceiptJson(const domain::Receipt& receipt);

}
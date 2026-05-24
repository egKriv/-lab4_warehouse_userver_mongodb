#pragma once
#include <domain/receipt.hpp>
#include <infrastructure/postgres_storage/postgres_storage.hpp>
#include <lib/json_builders/json_builders.hpp>
#include <userver/formats/json/value.hpp>

namespace inventory::services {

class ReceiptService {
public:
    explicit ReceiptService(storage::PostgresStorage& storage) : storage_(storage) {}

    std::optional<userver::formats::json::Value> Create(const domain::Receipt& receipt);
    userver::formats::json::Value GetByProductId(const std::string& product_id, int limit, int offset);
    userver::formats::json::Value GetAll(int limit, int offset);

private:
    storage::PostgresStorage& storage_;
};

}
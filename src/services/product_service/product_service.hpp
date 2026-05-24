#pragma once
#include <domain/product.hpp>
#include <infrastructure/postgres_storage/postgres_storage.hpp>
#include <lib/json_builders/json_builders.hpp>
#include <userver/formats/json/value.hpp>

namespace inventory::services {

class ProductService {
public:
    explicit ProductService(storage::PostgresStorage& storage) : storage_(storage) {}

    std::optional<userver::formats::json::Value> Add(const domain::Product& product);
    std::vector<userver::formats::json::Value> SearchByName(const std::string& name);
    userver::formats::json::Value GetStock(int limit, int offset);
    bool WriteOff(const std::string& product_id, int quantity);

private:
    storage::PostgresStorage& storage_;
};

}
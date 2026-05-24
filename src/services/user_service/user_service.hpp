#pragma once
#include <domain/user.hpp>
#include <infrastructure/postgres_storage/postgres_storage.hpp>
#include <lib/json_builders/json_builders.hpp>
#include <userver/formats/json/value.hpp>

namespace inventory::services {

class UserService {
public:
    explicit UserService(storage::PostgresStorage& storage) : storage_(storage) {}

    std::optional<userver::formats::json::Value> Register(const domain::User& user);
    std::optional<userver::formats::json::Value> GetByLogin(const std::string& login);
    std::vector<userver::formats::json::Value> SearchByNameMask(const std::string& mask);

private:
    storage::PostgresStorage& storage_;
};

}
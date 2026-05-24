#pragma once
#include <userver/components/component_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <optional>
#include <vector>
#include <domain/user.hpp>
#include <domain/product.hpp>
#include <domain/receipt.hpp>

namespace inventory::storage {

template<typename T>
struct Page {
    std::vector<T> items;
    int total = 0;
};

class PostgresStorage final : public userver::components::ComponentBase {
public:
    static constexpr std::string_view kName = "postgres-storage";

    PostgresStorage(const userver::components::ComponentConfig& config,
                    const userver::components::ComponentContext& context);

    // Users
    std::optional<domain::User> CreateUser(const domain::User& user);
    std::optional<domain::User> GetUserByLogin(const std::string& login);
    std::optional<domain::User> GetUserById(const std::string& id);
    std::vector<domain::User> SearchUsersByNameMask(const std::string& mask);

    // Products
    std::optional<domain::Product> CreateProduct(const domain::Product& product);
    std::optional<domain::Product> GetProductById(const std::string& id);
    std::vector<domain::Product> SearchProductsByName(const std::string& name_pattern);
    Page<domain::Product> GetAllProducts(int limit, int offset);
    bool UpdateProductQuantity(const std::string& id, int delta); // positive/negative

    // Receipts
    std::optional<domain::Receipt> CreateReceipt(const domain::Receipt& receipt);
    Page<domain::Receipt> GetReceiptsByProductId(const std::string& product_id, int limit, int offset);
    Page<domain::Receipt> GetAllReceipts(int limit, int offset);

private:
    userver::storages::postgres::ClusterPtr cluster_;
};

} // namespace inventory::storage
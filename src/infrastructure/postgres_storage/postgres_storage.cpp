#include "postgres_storage.hpp"
#include <userver/components/component_context.hpp>
#include <userver/storages/postgres/io/row_types.hpp>
#include <userver/storages/postgres/parameter_store.hpp>
#include <userver/storages/postgres/component.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <chrono>

namespace inventory::storage {

PostgresStorage::PostgresStorage(const userver::components::ComponentConfig& config,
                                 const userver::components::ComponentContext& context)
    : ComponentBase(config, context),
      cluster_(context.FindComponent<userver::components::Postgres>("postgres-db").GetCluster()) {}

// Users
std::optional<domain::User> PostgresStorage::CreateUser(const domain::User& user) {
    auto result = cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "INSERT INTO users (login, password_hash, first_name, last_name, email, role) "
        "VALUES ($1, $2, $3, $4, $5, $6) "
        "RETURNING id, login, password_hash, first_name, last_name, email, role, created_at",
        user.GetLogin(), user.GetPasswordHash(), user.GetFirstName(),
        user.GetLastName(), user.GetEmail(), user.GetRole());
    if (result.IsEmpty()) return std::nullopt;
    auto row = result.Front();
    return domain::User::FromDb(
        row["id"].As<boost::uuids::uuid>(),
        row["login"].As<std::string>(),
        row["password_hash"].As<std::string>(),
        row["first_name"].As<std::string>(),
        row["last_name"].As<std::string>(),
        row["email"].As<std::string>(),
        row["role"].As<std::string>(),
        row["created_at"].As<std::chrono::system_clock::time_point>()
    );
}

std::optional<domain::User> PostgresStorage::GetUserByLogin(const std::string& login) {
    auto result = cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, login, password_hash, first_name, last_name, email, role, created_at "
        "FROM users WHERE login = $1", login);
    if (result.IsEmpty()) return std::nullopt;
    auto row = result.Front();
    return domain::User::FromDb(
        row["id"].As<boost::uuids::uuid>(),
        row["login"].As<std::string>(),
        row["password_hash"].As<std::string>(),
        row["first_name"].As<std::string>(),
        row["last_name"].As<std::string>(),
        row["email"].As<std::string>(),
        row["role"].As<std::string>(),
        row["created_at"].As<std::chrono::system_clock::time_point>()
    );
}

std::optional<domain::User> PostgresStorage::GetUserById(const std::string& id) {
    auto result = cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, login, password_hash, first_name, last_name, email, role, created_at "
        "FROM users WHERE id = $1", boost::lexical_cast<boost::uuids::uuid>(id));
    if (result.IsEmpty()) return std::nullopt;
    auto row = result.Front();
    return domain::User::FromDb(
        row["id"].As<boost::uuids::uuid>(),
        row["login"].As<std::string>(),
        row["password_hash"].As<std::string>(),
        row["first_name"].As<std::string>(),
        row["last_name"].As<std::string>(),
        row["email"].As<std::string>(),
        row["role"].As<std::string>(),
        row["created_at"].As<std::chrono::system_clock::time_point>()
    );
}

std::vector<domain::User> PostgresStorage::SearchUsersByNameMask(const std::string& mask) {
    auto result = cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, login, password_hash, first_name, last_name, email, role, created_at "
        "FROM users WHERE first_name ILIKE $1 OR last_name ILIKE $1",
        "%" + mask + "%");
    std::vector<domain::User> users;
    for (const auto& row : result) {
        users.push_back(domain::User::FromDb(
            row["id"].As<boost::uuids::uuid>(),
            row["login"].As<std::string>(),
            row["password_hash"].As<std::string>(),
            row["first_name"].As<std::string>(),
            row["last_name"].As<std::string>(),
            row["email"].As<std::string>(),
            row["role"].As<std::string>(),
            row["created_at"].As<std::chrono::system_clock::time_point>()
        ));
    }
    return users;
}

// Products
std::optional<domain::Product> PostgresStorage::CreateProduct(const domain::Product& product) {
    auto result = cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "INSERT INTO products (name, description, sku, quantity_on_hand, unit) "
        "VALUES ($1, $2, $3, $4, $5) "
        "RETURNING id, name, description, sku, quantity_on_hand, unit, created_at, updated_at",
        product.GetName(), product.GetDescription(), product.GetSku(),
        product.GetQuantityOnHand(), product.GetUnit());
    if (result.IsEmpty()) return std::nullopt;
    auto row = result.Front();
    return domain::Product::FromDb(
        row["id"].As<boost::uuids::uuid>(),
        row["name"].As<std::string>(),
        row["description"].As<std::string>(),
        row["sku"].As<std::string>(),
        row["quantity_on_hand"].As<int>(),
        row["unit"].As<std::string>(),
        row["created_at"].As<std::chrono::system_clock::time_point>(),
        row["updated_at"].As<std::chrono::system_clock::time_point>()
    );
}

std::optional<domain::Product> PostgresStorage::GetProductById(const std::string& id) {
    auto result = cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, name, description, sku, quantity_on_hand, unit, created_at, updated_at "
        "FROM products WHERE id = $1",
        boost::lexical_cast<boost::uuids::uuid>(id));
    if (result.IsEmpty()) return std::nullopt;
    auto row = result.Front();
    return domain::Product::FromDb(
        row["id"].As<boost::uuids::uuid>(),
        row["name"].As<std::string>(),
        row["description"].As<std::string>(),
        row["sku"].As<std::string>(),
        row["quantity_on_hand"].As<int>(),
        row["unit"].As<std::string>(),
        row["created_at"].As<std::chrono::system_clock::time_point>(),
        row["updated_at"].As<std::chrono::system_clock::time_point>()
    );
}

std::vector<domain::Product> PostgresStorage::SearchProductsByName(const std::string& name_pattern) {
    auto result = cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, name, description, sku, quantity_on_hand, unit, created_at, updated_at "
        "FROM products WHERE name ILIKE $1",
        "%" + name_pattern + "%");
    std::vector<domain::Product> products;
    for (const auto& row : result) {
        products.push_back(domain::Product::FromDb(
            row["id"].As<boost::uuids::uuid>(),
            row["name"].As<std::string>(),
            row["description"].As<std::string>(),
            row["sku"].As<std::string>(),
            row["quantity_on_hand"].As<int>(),
            row["unit"].As<std::string>(),
            row["created_at"].As<std::chrono::system_clock::time_point>(),
            row["updated_at"].As<std::chrono::system_clock::time_point>()
        ));
    }
    return products;
}

Page<domain::Product> PostgresStorage::GetAllProducts(int limit, int offset) {
    auto count_res = cluster_->Execute(userver::storages::postgres::ClusterHostType::kSlave,
                                       "SELECT COUNT(*) FROM products");
    int total = count_res.AsSingleRow<int>();

    auto result = cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, name, description, sku, quantity_on_hand, unit, created_at, updated_at "
        "FROM products ORDER BY name LIMIT $1 OFFSET $2",
        limit, offset);
    Page<domain::Product> page;
    page.total = total;
    for (const auto& row : result) {
        page.items.push_back(domain::Product::FromDb(
            row["id"].As<boost::uuids::uuid>(),
            row["name"].As<std::string>(),
            row["description"].As<std::string>(),
            row["sku"].As<std::string>(),
            row["quantity_on_hand"].As<int>(),
            row["unit"].As<std::string>(),
            row["created_at"].As<std::chrono::system_clock::time_point>(),
            row["updated_at"].As<std::chrono::system_clock::time_point>()
        ));
    }
    return page;
}

bool PostgresStorage::UpdateProductQuantity(const std::string& id, int delta) {
    auto result = cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "UPDATE products SET quantity_on_hand = quantity_on_hand + $1, updated_at = NOW() "
        "WHERE id = $2 AND quantity_on_hand + $1 >= 0",
        delta, boost::lexical_cast<boost::uuids::uuid>(id));
    return result.RowsAffected() > 0;
}

// Receipts
std::optional<domain::Receipt> PostgresStorage::CreateReceipt(const domain::Receipt& receipt) {
    auto result = cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kMaster,
        "INSERT INTO receipts (product_id, quantity, supplier, created_by) "
        "VALUES ($1, $2, $3, $4) "
        "RETURNING id, product_id, quantity, supplier, receipt_date, created_by",
        receipt.GetProductId(), receipt.GetQuantity(), receipt.GetSupplier(), receipt.GetCreatedBy());
    if (result.IsEmpty()) return std::nullopt;
    auto row = result.Front();
    return domain::Receipt::FromDb(
        row["id"].As<boost::uuids::uuid>(),
        row["product_id"].As<boost::uuids::uuid>(),
        row["quantity"].As<int>(),
        row["supplier"].As<std::string>(),
        row["receipt_date"].As<std::chrono::system_clock::time_point>(),
        row["created_by"].As<std::string>()
    );
}

Page<domain::Receipt> PostgresStorage::GetReceiptsByProductId(const std::string& product_id, int limit, int offset) {
    auto count_res = cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT COUNT(*) FROM receipts WHERE product_id = $1",
        boost::lexical_cast<boost::uuids::uuid>(product_id));
    int total = count_res.AsSingleRow<int>();

    auto result = cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, product_id, quantity, supplier, receipt_date, created_by "
        "FROM receipts WHERE product_id = $1 ORDER BY receipt_date DESC LIMIT $2 OFFSET $3",
        boost::lexical_cast<boost::uuids::uuid>(product_id), limit, offset);
    Page<domain::Receipt> page;
    page.total = total;
    for (const auto& row : result) {
        page.items.push_back(domain::Receipt::FromDb(
            row["id"].As<boost::uuids::uuid>(),
            row["product_id"].As<boost::uuids::uuid>(),
            row["quantity"].As<int>(),
            row["supplier"].As<std::string>(),
            row["receipt_date"].As<std::chrono::system_clock::time_point>(),
            row["created_by"].As<std::string>()
        ));
    }
    return page;
}

Page<domain::Receipt> PostgresStorage::GetAllReceipts(int limit, int offset) {
    auto count_res = cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT COUNT(*) FROM receipts");
    int total = count_res.AsSingleRow<int>();

    auto result = cluster_->Execute(
        userver::storages::postgres::ClusterHostType::kSlave,
        "SELECT id, product_id, quantity, supplier, receipt_date, created_by "
        "FROM receipts ORDER BY receipt_date DESC LIMIT $1 OFFSET $2",
        limit, offset);
    Page<domain::Receipt> page;
    page.total = total;
    for (const auto& row : result) {
        page.items.push_back(domain::Receipt::FromDb(
            row["id"].As<boost::uuids::uuid>(),
            row["product_id"].As<boost::uuids::uuid>(),
            row["quantity"].As<int>(),
            row["supplier"].As<std::string>(),
            row["receipt_date"].As<std::chrono::system_clock::time_point>(),
            row["created_by"].As<std::string>()
        ));
    }
    return page;
}

} // namespace inventory::storage
#pragma once

#include <string>
#include <chrono>
#include <variant>
#include <optional>
#include <boost/uuid/uuid.hpp>
#include "exceptions.hpp"

namespace inventory::domain {

class Product {
public:
    static std::variant<exceptions::domain::ValidationError, Product> Create(
        const std::string& name, const std::string& description, const std::string& sku,
        int initial_quantity, const std::string& unit = "pcs"
    ) {
        if (name.empty()) return exceptions::domain::ValidationError{"name", "Product name cannot be empty"};
        if (initial_quantity < 0) return exceptions::domain::ValidationError{"initial_quantity", "Quantity cannot be negative"};
        Product p;
        p.name_ = name;
        p.description_ = description;
        p.sku_ = sku;
        p.quantity_on_hand_ = initial_quantity;
        p.unit_ = unit;
        return p;
    }

    static Product FromDb(const boost::uuids::uuid& id, const std::string& name, const std::string& description,
                          const std::string& sku, int quantity_on_hand, const std::string& unit,
                          const std::chrono::system_clock::time_point& created_at,
                          const std::chrono::system_clock::time_point& updated_at) {
        Product p;
        p.id_ = id;
        p.name_ = name;
        p.description_ = description;
        p.sku_ = sku;
        p.quantity_on_hand_ = quantity_on_hand;
        p.unit_ = unit;
        p.created_at_ = created_at;
        p.updated_at_ = updated_at;
        return p;
    }

    const boost::uuids::uuid& GetId() const { return id_; }
    const std::string& GetName() const { return name_; }
    const std::string& GetDescription() const { return description_; }
    const std::string& GetSku() const { return sku_; }
    int GetQuantityOnHand() const { return quantity_on_hand_; }
    const std::string& GetUnit() const { return unit_; }

private:
    boost::uuids::uuid id_;
    std::string name_;
    std::string description_;
    std::string sku_;
    int quantity_on_hand_ = 0;
    std::string unit_;
    std::chrono::system_clock::time_point created_at_;
    std::chrono::system_clock::time_point updated_at_;
};

} // namespace inventory::domain
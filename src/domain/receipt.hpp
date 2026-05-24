#pragma once
#include <string>
#include <chrono>
#include <boost/uuid/uuid.hpp>
#include "exceptions.hpp"

namespace inventory::domain {

class Receipt {
public:
    static Receipt Create(const boost::uuids::uuid& product_id, int quantity,
                          const std::string& supplier, const std::string& created_by) {
        Receipt r;
        r.product_id_ = product_id;
        r.quantity_ = quantity;
        r.supplier_ = supplier;
        r.created_by_ = created_by;
        return r;
    }

    static Receipt FromDb(const boost::uuids::uuid& id, const boost::uuids::uuid& product_id,
                          int quantity, const std::string& supplier,
                          const std::chrono::system_clock::time_point& receipt_date,
                          const std::string& created_by) {
        Receipt r;
        r.id_ = id;
        r.product_id_ = product_id;
        r.quantity_ = quantity;
        r.supplier_ = supplier;
        r.receipt_date_ = receipt_date;
        r.created_by_ = created_by;
        return r;
    }

    const boost::uuids::uuid& GetId() const { return id_; }
    const boost::uuids::uuid& GetProductId() const { return product_id_; }
    int GetQuantity() const { return quantity_; }
    const std::string& GetSupplier() const { return supplier_; }
    const std::chrono::system_clock::time_point& GetReceiptDate() const { return receipt_date_; }
    const std::string& GetCreatedBy() const { return created_by_; }

private:
    boost::uuids::uuid id_;
    boost::uuids::uuid product_id_;
    int quantity_ = 0;
    std::string supplier_;
    std::chrono::system_clock::time_point receipt_date_;
    std::string created_by_;
};

} // namespace inventory::domain
#pragma once
#include <string>
#include <regex>
#include <chrono>
#include <variant>
#include <boost/uuid/uuid.hpp>
#include <lib/password_hasher/password_hasher.hpp>
#include "exceptions.hpp"

namespace inventory::domain {

class User {
public:
    static std::variant<exceptions::domain::ValidationError, User> Create(
        const std::string& login,
        const std::string& password,
        const std::string& first_name,
        const std::string& last_name,
        const std::string& email,
        const std::string& role = "storekeeper"
    ) {
        if (login.length() < 3 || login.length() > 50) return exceptions::domain::ValidationError{"login", "Login must be 3-50 characters"};
        if (password.length() < 6) return exceptions::domain::ValidationError{"password", "Password must be at least 6 characters"};
        if (first_name.empty()) return exceptions::domain::ValidationError{"first_name", "First name required"};
        if (last_name.empty()) return exceptions::domain::ValidationError{"last_name", "Last name required"};
        if (email.find('@') == std::string::npos) return exceptions::domain::ValidationError{"email", "Invalid email"};

        auto hasher = lab2::infrastructure::PasswordHasher();
        User user;
        user.login_ = login;
        user.password_hash_ = hasher.Hash(password);
        user.first_name_ = first_name;
        user.last_name_ = last_name;
        user.email_ = email;
        user.role_ = role;
        return user;
    }

    static User FromDb(const boost::uuids::uuid& id, const std::string& login, const std::string& password_hash,
                       const std::string& first_name, const std::string& last_name, const std::string& email,
                       const std::string& role, const std::chrono::system_clock::time_point& created_at) {
        User user;
        user.id_ = id;
        user.login_ = login;
        user.password_hash_ = password_hash;
        user.first_name_ = first_name;
        user.last_name_ = last_name;
        user.email_ = email;
        user.role_ = role;
        user.created_at_ = created_at;
        return user;
    }

    const boost::uuids::uuid& GetId() const { return id_; }
    const std::string& GetLogin() const { return login_; }
    const std::string& GetPasswordHash() const { return password_hash_; }
    const std::string& GetFirstName() const { return first_name_; }
    const std::string& GetLastName() const { return last_name_; }
    const std::string& GetEmail() const { return email_; }
    const std::string& GetRole() const { return role_; }
    const std::chrono::system_clock::time_point& GetCreatedAt() const { return created_at_; }

private:
    boost::uuids::uuid id_;
    std::string login_;
    std::string password_hash_;
    std::string first_name_;
    std::string last_name_;
    std::string email_;
    std::string role_;
    std::chrono::system_clock::time_point created_at_;
};

} // namespace inventory::domain
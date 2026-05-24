#include "user_service.hpp"

namespace inventory::services {

std::optional<userver::formats::json::Value> UserService::Register(const domain::User& user) {
    auto stored = storage_.CreateUser(user);
    if (!stored) return std::nullopt;
    return json_builders::BuildUserJson(*stored);
}

std::optional<userver::formats::json::Value> UserService::GetByLogin(const std::string& login) {
    auto user = storage_.GetUserByLogin(login);
    if (!user) return std::nullopt;
    return json_builders::BuildUserJson(*user);
}

std::vector<userver::formats::json::Value> UserService::SearchByNameMask(const std::string& mask) {
    auto users = storage_.SearchUsersByNameMask(mask);
    std::vector<userver::formats::json::Value> result;
    for (const auto& u : users) {
        result.push_back(json_builders::BuildUserJson(u));
    }
    return result;
}

}
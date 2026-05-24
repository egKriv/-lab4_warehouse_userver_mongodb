#include "search_users.hpp"
#include <userver/server/handlers/exceptions.hpp>
#include <domain/exceptions.hpp>
#include <lib/json_builders/json_builders.hpp>

namespace inventory::controllers {

SearchUsers::SearchUsers(const userver::components::ComponentConfig& config,
                         const userver::components::ComponentContext& context)
    : HttpHandlerJsonBase(config, context),
      user_service_(context.FindComponent<inventory::storage::PostgresStorage>()) {}

userver::formats::json::Value SearchUsers::HandleRequestJsonThrow(
    const userver::server::http::HttpRequest& request,
    const userver::formats::json::Value& /*json*/,
    userver::server::request::RequestContext& /*context*/) const {
    const auto& name_mask = request.GetArg("name_mask");
    if (name_mask.empty()) {
        throw userver::server::handlers::ClientError(userver::server::handlers::ExternalBody{
            R"({"code":"validation_error","message":"name_mask query parameter is required"})"});
    }
    auto users = user_service_.SearchByNameMask(name_mask);
    userver::formats::json::ValueBuilder builder;
    userver::formats::json::ValueBuilder items;
    for (const auto& u : users) {
        items.PushBack(u);
    }
    builder["items"] = std::move(items);
    builder["total"] = static_cast<int>(users.size());
    return builder.ExtractValue();
}

} // namespace inventory::controllers
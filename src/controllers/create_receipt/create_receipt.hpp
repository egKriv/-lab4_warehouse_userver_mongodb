#pragma once
#include <userver/server/handlers/http_handler_json_base.hpp>
#include <userver/components/component_context.hpp>
#include <services/receipt_service/receipt_service.hpp>

namespace inventory::controllers {

class CreateReceipt final : public userver::server::handlers::HttpHandlerJsonBase {
public:
    static constexpr std::string_view kName = "HandlerCreateReceipt";
    CreateReceipt(const userver::components::ComponentConfig& config,
                  const userver::components::ComponentContext& context);

    userver::formats::json::Value HandleRequestJsonThrow(
        const userver::server::http::HttpRequest& request,
        const userver::formats::json::Value& json,
        userver::server::request::RequestContext& context) const override;

private:
    mutable inventory::services::ReceiptService receipt_service_;
};

} // namespace inventory::controllers
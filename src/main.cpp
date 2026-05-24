#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component_list.hpp>
#include <userver/components/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/storages/mongo/component.hpp>
#include <userver/congestion_control/component.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/utils/daemon_run.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <infrastructure/jwt/jwt_auth_factory.hpp>
#include <controllers/create_user/create_user.hpp>
#include <controllers/login/login.hpp>
#include <controllers/get_user_by_login/get_user_by_login.hpp>
#include <controllers/search_users/search_users.hpp>
#include <controllers/add_product/add_product.hpp>
#include <controllers/search_products/search_products.hpp>
#include <controllers/get_stock/get_stock.hpp>
#include <controllers/create_receipt/create_receipt.hpp>
#include <controllers/get_receipts/get_receipts.hpp>
#include <controllers/write_off/write_off.hpp>
#include <controllers/get_writeoffs/get_writeoffs.hpp>

int main(int argc, char* argv[]) {
    userver::server::handlers::auth::RegisterAuthCheckerFactory<
        inventory::infrastructure::JwtAuthCheckerFactory>();

    auto component_list = userver::components::MinimalServerComponentList()
        .Append<userver::congestion_control::Component>()
        .Append<userver::components::TestsuiteSupport>()
        .AppendComponentList(userver::clients::http::ComponentList())
        .Append<userver::clients::dns::Component>()
        .Append<userver::components::Postgres>("postgres-db")
        .Append<userver::components::Mongo>("mongo-db")
        .Append<inventory::storage::PostgresStorage>()
        .Append<inventory::infrastructure::JwtAuthComponent>()
        .Append<userver::server::handlers::Ping>()
        .Append<inventory::controllers::CreateUser>()
        .Append<inventory::controllers::Login>()
        .Append<inventory::controllers::GetUserByLogin>()
        .Append<inventory::controllers::SearchUsers>()
        .Append<inventory::controllers::AddProduct>()
        .Append<inventory::controllers::SearchProducts>()
        .Append<inventory::controllers::GetStock>()
        .Append<inventory::controllers::CreateReceipt>()
        .Append<inventory::controllers::GetReceipts>()
        .Append<inventory::controllers::WriteOff>()
        .Append<inventory::controllers::GetWriteoffs>();

    return userver::utils::DaemonMain(argc, argv, component_list);
}
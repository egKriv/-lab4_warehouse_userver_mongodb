#pragma once

#include <userver/components/loggable_component_base.hpp>
#include <userver/yaml_config/merge_schemas.hpp>
#include <userver/yaml_config/schema.hpp>
#include <memory>
#include "jwt_generator.hpp"
#include "jwt_checker.hpp"

namespace inventory::infrastructure {

class JwtAuthComponent final : public userver::components::LoggableComponentBase {
public:
    static constexpr std::string_view kName = "jwt-auth-component";

    static userver::yaml_config::Schema GetStaticConfigSchema() {
        return userver::yaml_config::MergeSchemas<userver::components::LoggableComponentBase>(R"(
            type: object
            description: JWT authentication component
            additionalProperties: false
            properties:
                private_key:
                    type: string
                    description: secret key for JWT signing
                issuer:
                    type: string
                    description: JWT issuer
                audience:
                    type: string
                    description: JWT audience
        )");
    }

    JwtAuthComponent(const userver::components::ComponentConfig& config,
                     const userver::components::ComponentContext& context);

    std::shared_ptr<JwtGenerator> GetGenerator() const;
    std::shared_ptr<JwtChecker> GetChecker() const;

private:
    std::shared_ptr<JwtGenerator> generator_;
    std::shared_ptr<JwtChecker> checker_;
};

} // namespace inventory::infrastructure
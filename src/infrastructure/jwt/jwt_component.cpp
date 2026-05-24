#include "jwt_component.hpp"
#include <userver/components/component_config.hpp>
#include <userver/components/component_context.hpp>

namespace inventory::infrastructure {

JwtAuthComponent::JwtAuthComponent(const userver::components::ComponentConfig& config,
                                   const userver::components::ComponentContext& context)
    : LoggableComponentBase(config, context) {
    auto cfg = config;
    generator_ = std::make_shared<JwtGenerator>(
        cfg["private_key"].As<std::string>(),
        cfg["issuer"].As<std::string>(),
        cfg["audience"].As<std::string>()
    );
    checker_ = std::make_shared<JwtChecker>(
        cfg["private_key"].As<std::string>(),
        cfg["issuer"].As<std::string>(),
        cfg["audience"].As<std::string>()
    );
}

std::shared_ptr<JwtGenerator> JwtAuthComponent::GetGenerator() const {
    return generator_;
}

std::shared_ptr<JwtChecker> JwtAuthComponent::GetChecker() const {
    return checker_;
}

} // namespace inventory::infrastructure
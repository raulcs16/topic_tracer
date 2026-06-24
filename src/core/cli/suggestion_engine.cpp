#include "suggestion_engine.hpp"

void SuggestionEngine::registerProvider(const std::string &key,
                                        std::unique_ptr<IProvider> prv) {
    if (hasProvider(key))
        return;
    m_providers[key] = std::move(prv);
}
bool SuggestionEngine::hasProvider(const std::string &name) const {
    auto it = m_providers.find(name);
    return !(it == m_providers.end());
}
std::vector<std::string> SuggestionEngine::matches(const std::string &provider,
                                                   const std::string &pattern) {
    if (!hasProvider(provider))
        return {};
    return m_providers[provider]->matches(pattern);
}

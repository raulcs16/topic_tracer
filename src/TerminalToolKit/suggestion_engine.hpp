#pragma once
#include "iprovider.hpp"
#include <map>

class SuggestionEngine {

public:
    SuggestionEngine() {}
    void registerProvider(const std::string &name, std::unique_ptr<IProvider> prv);
    bool hasProvider(const std::string &name) const;
    size_t providerSize() const { return m_providers.size(); }
    std::vector<std::string> matches(const std::string &provider,
                                     const std::string &pattern);

private:
    std::map<std::string, std::unique_ptr<IProvider>> m_providers;
};
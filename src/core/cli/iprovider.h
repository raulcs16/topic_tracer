#pragma once

#include <string>
#include <vector>


class IProvider {
public:
    virtual ~IProvider() = default;
    virtual std::vector<std::string> matches(const std::string &) = 0;
};
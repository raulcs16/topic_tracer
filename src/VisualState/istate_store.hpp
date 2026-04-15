#pragma once
#include "geometry.hpp"
#include "visual_states.hpp"
#include "visual_types.hpp"

class IStateStore {
public:
    virtual ~IStateStore() = default;
    //Getters
    virtual QString label(uint32_t id) const = 0;
    virtual QStringList labels() const = 0;

    virtual StateFlags nodeFlags(uint32_t id) const = 0;
    virtual visual::Point nodePos(uint32_t id) const = 0;

    virtual visual::Line edgePos(const std::string &key) const = 0;
    virtual StateFlag edgeFlags(const std::string &key) const = 0;
    //virtual EdgeType edgeType(const std::string &key) const = 0;
    virtual QRectF rect(uint32_t clusterId) const = 0;


    //Setters
}
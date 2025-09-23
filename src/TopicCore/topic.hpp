#pragma once


#include "topic_type.hpp"
#include <cstdint>
#include <string>

class Topic {

public:
    explicit Topic(uint32_t id,
                   const std::string &name,
                   Topic_Type type,
                   bool covered = false);

    //getters
    uint32_t id() const;
    const std::string &name() const;
    bool covered() const;
    Topic_Type type() const;
    //setters
    void setName(const std::string &name);
    void setCovered(bool is_covered);


private:
    uint32_t m_id;
    std::string m_name;
    bool m_covered;
    Topic_Type m_type;
};
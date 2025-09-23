#include "topic.hpp"


Topic::Topic(uint32_t id, const std::string &name, Topic_Type type, bool covered)
    : m_id{id}, m_name{name}, m_type{type}, m_covered{covered} {}

uint32_t Topic::id() const { return m_id; }
const std::string &Topic::name() const { return m_name; }
bool Topic::covered() const { return m_covered; }
Topic_Type Topic::type() const { return m_type; }
void Topic::setName(const std::string &name) { m_name = name; }
void Topic::setCovered(bool is_covered) { m_covered = is_covered; }

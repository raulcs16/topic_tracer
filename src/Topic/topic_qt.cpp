#include "topic_qt.hpp"


uint32_t TopicQt::id() const { return m_topic->getId(); }
QString TopicQt::name() const {
    QString name = QString::fromStdString(m_topic->getName());
    return name;
}
bool TopicQt::isCovered() const { return m_topic->isCovered(); }
Topic_Type TopicQt::type() const { return m_topic->getType(); }

//slots
void TopicQt::setCovered(bool is_covered) {
    if (m_topic->isCovered() == is_covered)
        return;
    m_topic->setCovered(is_covered);
    emit coveredChanged(is_covered);
}
void TopicQt::setName(QString name) {
    std::string new_name = name.toStdString();
    m_topic->setName(new_name);
    emit nameChanged(name);
}

//signals::
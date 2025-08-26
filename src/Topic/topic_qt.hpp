#pragma once
#include "topic.hpp"
#include <QObject>

class TopicQt : public QObject {
    Q_OBJECT
    Q_PROPERTY(uint32_t id READ id CONSTANT)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool covered READ isCovered WRITE setCovered NOTIFY coveredChanged)
    Q_PROPERTY(Topic_Type type READ type CONSTANT)


public:
    explicit TopicQt(const std::shared_ptr<Topic> &topic, QObject *parent = nullptr)
        : QObject(parent), m_topic(topic) {}
    uint32_t id() const;
    QString name() const;
    bool isCovered() const;
    Topic_Type type() const;

public slots:
    void setCovered(bool is_covered);
    void setName(QString name);
signals:
    void coveredChanged(bool covered);
    void nameChanged(QString name);

private:
    std::shared_ptr<Topic> m_topic;
};
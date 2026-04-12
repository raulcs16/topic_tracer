#pragma once

#include "graph_store.hpp"
#include <QObject>
#include <QString>
#include <QStringList>
#include <QtQml/qqml.h>

struct AutoSuggestion {
    int startIndex = -1;
    QString suggestion;
};

class CommandAutoCompleter : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Created in main.cpp")
public:
    explicit CommandAutoCompleter(QStringList verbs,
                                  GraphStore *store,
                                  QObject *parent = nullptr)
        : m_verbs(verbs), m_store(store), QObject{parent} {}
    QList<AutoSuggestion> findMatches(const QString &input) const;

private:
    GraphStore *m_store;
    QStringList m_verbs;
};
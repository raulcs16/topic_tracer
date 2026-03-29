#pragma once

#include "graph_store.hpp"
#include <QObject>
#include <QString>
#include <QStringList>
#include <QtQml/qqml.h>

class CommandAutoCompleter : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Created in main.cpp")
public:
    explicit CommandAutoCompleter(GraphStore *store, QObject *parent = nullptr)
        : m_store(store), QObject{parent} {
        m_verbs = {"clear",
                   "save",
                   "load",
                   "touch",
                   "link",
                   "rm",
                   "path",
                   "mv",
                   "focus",
                   "mode"};
        m_edgeTypes = {"composes", "aggregates", "associates", "injects", "implements"};
    }
    Q_INVOKABLE QString complete(const QString &raw_input) const;


private:
    QString findBestMatch(const QString &partial, const QStringList &canidates) const;

private:
    GraphStore *m_store;
    QStringList m_verbs;
    QStringList m_edgeTypes;
};
#pragma once

#include "terminal_list_model.hpp"
#include <QObject>
#include <QString>
#include <QtQml/qqml.h>


class TerminalController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Passed in Property")

    Q_PROPERTY(TerminalListModel *terminalListModel READ terminalListModel CONSTANT)
    Q_PROPERTY(QString suggestion READ suggestion NOTIFY suggestionChanged)

public:
    explicit TerminalController(TerminalListModel *model, QObject *parent = nullptr)
        : QObject{parent}, m_model{model} {}

    virtual ~TerminalController() = default;
    QString suggestion() { return m_suggestion; }

    Q_INVOKABLE virtual void handleEnter(const QString &raw_input) = 0;
    Q_INVOKABLE virtual void handleTab(const QString &raw_input) = 0;
    Q_INVOKABLE virtual void handleChange(const QString &raw_input) = 0;

    TerminalListModel *terminalListModel() const { return m_model; }
signals:
    void suggestionChanged();

protected:
    QString m_suggestion = "";
    TerminalListModel *m_model;
};
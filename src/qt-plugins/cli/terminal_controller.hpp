#pragma once

#include "get_auto_suggestions.hpp"
#include "handle_input.hpp"
#include <QObject>
#include <QString>
#include <QtQml/qqml.h>


class TerminalController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Passed in Property")

public:
    explicit TerminalController(GetAutoSuggestion *autoS,
                                HandleInput *input,
                                QObject *parent = nullptr)
        : QObject{parent}, m_auto{autoS}, m_input{input} {}
    virtual ~TerminalController() = default;

    Q_INVOKABLE void handleEnter(const QString &raw_input) {
        m_input->execute(raw_input.toStdString());
    }
    Q_INVOKABLE void handleTab(const QString &raw_input) {
        m_auto->execute(raw_input.toStdString());
    }
    Q_INVOKABLE void handleChange(const QString &raw_input) {}

private:
    GetAutoSuggestion *m_auto;
    HandleInput *m_input;
};
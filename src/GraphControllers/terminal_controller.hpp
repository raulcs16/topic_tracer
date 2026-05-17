#pragma once

#include "cli_service.hpp"
#include "terminal_list_model.hpp"
#include <QObject>
#include <QtQml/qqml.h>


class TerminalController : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Created in main.cpp")

public:
    explicit TerminalController(TerminalListModel *uiModel,
                                std::unique_ptr<CLIService> cliService,
                                QObject *parent = nullptr);

    Q_INVOKABLE void executeCommand(const QString &raw_cmd);
    Q_INVOKABLE void handleSuggestion(const QString &input);

private:
    TerminalListModel *m_terminal;
    std::unique_ptr<CLIService> m_cli;
};
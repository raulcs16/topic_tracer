#pragma once

#include "cli_service.hpp"
#include "terminal_controller.hpp"
#include <QObject>
#include <QtQml/qqml.h>


class CLITerminalController : public TerminalController {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Created in main.cpp")

public:
    explicit CLITerminalController(std::unique_ptr<CLIService> cliService,
                                   TerminalListModel *uiModel,
                                   QObject *parent = nullptr);

    Q_INVOKABLE void handleEnter(const QString &raw_input) override;
    Q_INVOKABLE void handleTab(const QString &raw_input) override;
    Q_INVOKABLE void handleChange(const QString &raw_input) override;

private:
    std::unique_ptr<CLIService> m_cli;
};
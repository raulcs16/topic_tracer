#include "terminal_controller.hpp"


TerminalController::TerminalController(TerminalListModel *uiModel,
                                       std::unique_ptr<CLIService> cliService,
                                       QObject *parent)
    : QObject{parent}, m_terminal{uiModel}, m_cli{std::move(cliService)} {
    m_terminal->addEntry("Welcome", EntryType::Success);
}

void TerminalController::executeCommand(const QString &raw_cmd) {
    if (raw_cmd.isEmpty())
        return;
}
void TerminalController::handleSuggestion(const QString &input) {}
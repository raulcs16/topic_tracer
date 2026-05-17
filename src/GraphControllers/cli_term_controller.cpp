#include "cli_term_controller.hpp"


CLITerminalController::CLITerminalController(std::unique_ptr<CLIService> cliService,
                                             TerminalListModel *uiModel,
                                             QObject *parent)
    : TerminalController{uiModel, parent}, m_cli{std::move(cliService)} {
    m_suggestion = "?";
    emit suggestionChanged();
    m_model->addEntry("Welcome", EntryType::Success);
}

void CLITerminalController::handleEnter(const QString &raw_input) {}
void CLITerminalController::handleTab(const QString &raw_input) {}
void CLITerminalController::handleChange(const QString &raw_input) {
    m_suggestion = "abcdefg";
    emit suggestionChanged();
}
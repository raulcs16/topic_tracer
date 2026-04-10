#pragma once

#include "icommand.hpp"
#include "ui_context.hpp"


class ModeCommand : public ICommand {
public:
    explicit ModeCommand(UIContext *context, QStringList parts)
        : m_uiContext(context), m_parts(parts) {}
    CommandResult execute() override {
        if (m_parts.length() != 2) {
            return CommandResult::error("Usage: mode <'progress|stress'>");
        }
        if (m_parts.at(1).toLower() == "progress") {
            m_uiContext->setMode(UIContext::ViewMode::Progress);
            return CommandResult::ok("");
        } else if (m_parts.at(1).toLower() == "stress") {
            m_uiContext->setMode(UIContext::ViewMode::Stress);
            return CommandResult::ok("");
        }
        return CommandResult::error("Usage: mode <'progress|stress'>");
    }
    void undo() override {}

private:
    UIContext *m_uiContext;
    QStringList m_parts;
};
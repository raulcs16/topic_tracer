#pragma once

#include "icommand.hpp"
#include "ui_context.hpp"


class ModeCommand : public ICommand {
public:
    explicit ModeCommand(UIContext *context, QString mode)
        : m_uiContext(context), m_mode(mode) {}
    void execute() override {
        if (m_mode.toLower() == "progress") {
            m_uiContext->setMode(UIContext::ViewMode::Progress);
        } else if (m_mode.toLower() == "stress") {
            m_uiContext->setMode(UIContext::ViewMode::Stress);
        }
    }
    void undo() override {}

private:
    UIContext *m_uiContext;
    QString m_mode;
};
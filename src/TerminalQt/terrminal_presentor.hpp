#pragma once

#include "terminal_out.hpp"
#include <QObject>
#include <QString>
#include <QtQml/qqml.h>
#include <unordered_map>
#include <vector>


class TerminalPresentor : public QObject public TerminalOut {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Created by ui context")
public:
    TerminalPresentor() {}
    void presentLine(const std::string &line) {}
    void presentList(std::vector<std::string> list) override {
        m_list.clear();
        for (auto &item : list) {
            m_list.push_back(QString(item));
        }
        emit listChanged();
    }

    void presentSuggestion(const std::string &suggestion) override {
        m_suggestion = QString(suggestion);
        emit suggestionChanged();
    }
signals:
    void suggestionChanged();
    void listChanged();

private:
    QString m_suggestion;
    std::vector<QString> m_list;
    std::unordered_map<size_t, QString> m_lines;
};
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
    void presentLine(const std::string &line) {
        size_t id = m_idx++;
        m_lines[id] = QString(line);
        emit lineAdded(id);
    }
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
    QString suggestion() const { return m_suggestion; }
    std::vector<QString> list() const { return m_list; }
    QString line(size_t id) const { return m_lines[id]; }


signals:
    void suggestionChanged();
    void listChanged();
    void lineAdded(size_t id);

private:
    QString m_suggestion;
    std::vector<QString> m_list;
    std::unordered_map<size_t, QString> m_lines;
    size_t m_idx = 1;
};
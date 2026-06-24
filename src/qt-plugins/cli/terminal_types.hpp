#pragma once

#include <QString>
enum class EntryType {
    Command,
    Output,
    Error,
    Hint,
    Success
};
struct TerminalEntry {
    EntryType type;
    QString entry;
};
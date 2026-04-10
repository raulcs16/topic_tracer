#include "terminal_types.hpp"
#include <QString>

struct CommandResult {
    bool success;
    QString message;
    EntryType type = EntryType::Output;

    static CommandResult ok(QString msg = "") { return {true, msg, EntryType::Output}; }
    static CommandResult error(QString msg) { return {false, msg, EntryType::Error}; }
};
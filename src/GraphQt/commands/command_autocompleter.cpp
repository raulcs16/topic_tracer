#include "command_autocompleter.hpp"

QString CommandAutoCompleter::complete(const QString &raw_input) const {
    QString trimmed = raw_input.trimmed();
    QStringList parts = trimmed.split(" ", Qt::SkipEmptyParts);

    if (parts.size() == 1) {
        return findBestMatch(parts.first(), m_verbs);
    }
    return raw_input;
}
QString CommandAutoCompleter::findBestMatch(const QString &partial,
                                            const QStringList &candidates) const {
    for (const auto &candidate : candidates) {
        if (candidate.startsWith(partial, Qt::CaseInsensitive)) {
            return candidate;
        }
    }
    return partial;
}
// QString AppController::getAutoComplete(QString raw_cmd) {
//     raw_cmd = raw_cmd.trimmed();
//     QStringList parts = raw_cmd.split(" ", Qt::SkipEmptyParts);
//     static QStringList commands =
//         {"clear", "save", "load", "touch", "link", "rm", "path", "mv", "focus", "mode"};
//     if (parts.empty())
//         return raw_cmd;
//     //autocomplete suggests a cmd
//     if (parts.size() == 1) {
//         QString partial = parts.takeFirst();
//         for (auto &cmd : commands)
//             if (cmd.startsWith(partial))
//                 return cmd; //TODO: return all matches
//         return raw_cmd;
//     }
//     if (parts[0] == "link") {
//         if (parts.size() == 5) {
//             QString partial = parts.takeLast();
//             QStringList type = {"composes",
//                                 "aggregates",
//                                 "associates",
//                                 "injects",
//                                 "implements"};
//             for (auto &t : type)
//                 if (t.startsWith(partial)) {
//                     parts.append(t);
//                     return parts.join(" ");
//                 }
//         }
//     }
//     QString partial = parts.takeLast();
//     QString match = m_store->findMatch(partial);
//     if (!match.isEmpty()) {
//         parts.append(match);
//         return parts.join(" ");
//     }

//     return raw_cmd;
// }
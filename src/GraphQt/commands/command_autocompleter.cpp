#include "command_autocompleter.hpp"

QList<AutoSuggestion> CommandAutoCompleter::findMatches(const QString &raw_input) const {
    QString trimmed = raw_input.trimmed();
    QStringList parts = raw_input.split(" ", Qt::SkipEmptyParts);
    QList<AutoSuggestion> results;

    if (parts.isEmpty())
        return results;

    QString partial = parts.last();
    int startIndex = raw_input.lastIndexOf(partial);
    QStringList candidates = (parts.size() == 1) ? m_verbs : m_store->labels();
    for (const auto &candidate : candidates) {
        if (candidate.startsWith(partial, Qt::CaseInsensitive)) {
            results.append({startIndex, candidate});
        }
    }
    return results;
}

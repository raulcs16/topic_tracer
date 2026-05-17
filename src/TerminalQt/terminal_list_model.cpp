#include "terminal_list_model.hpp"

TerminalListModel::TerminalListModel(QObject *parent) : QAbstractListModel(parent) {}

QHash<int, QByteArray> TerminalListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[TextRole] = "text";
    roles[EntryTypeRole] = "type";
    return roles;
}
int TerminalListModel::rowCount(const QModelIndex &parent) const {
    return m_entries.size();
}
QVariant TerminalListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();
    const auto &entry = m_entries[index.row()];
    if (TextRole == role)
        return entry.entry;
    if (EntryTypeRole == role)
        return static_cast<int>(entry.type);
    return QVariant();
}
Qt::ItemFlags TerminalListModel::flags(const QModelIndex &index) const {
    return QAbstractListModel::flags(index);
}

void TerminalListModel::addEntry(const QString &entry, EntryType type) {
    int index = m_entries.size();
    beginInsertRows(QModelIndex(), index, index);
    m_entries.push_back(TerminalEntry{.type = type, .entry = entry});
    endInsertRows();
}
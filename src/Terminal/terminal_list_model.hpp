#include <QAbstractListModel>
#include <QObject>
#include <QtQml/qqml.h>

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


class TerminalListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
public:
    enum Roles {
        TextRole = Qt::UserRole + 1,
        EntryTypeRole,
    };
    explicit TerminalListModel(QObject *parent = nullptr);
    //abstractlistmodel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    Q_INVOKABLE
    void addEntry(const QString &entry, EntryType type = EntryType::Command);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<TerminalEntry> m_entries;
};
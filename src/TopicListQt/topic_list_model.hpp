#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QtQml/qqml.h>


struct TopicItem {
    uint32_t id;
    QString name;
};

class TopicListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
    };
    explicit TopicListModel(QObject *parent = nullptr);

    //list interface override
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    //API
    Q_INVOKABLE void addItem(const QString &name);
    Q_INVOKABLE bool removeItem(int index);
    Q_INVOKABLE bool editItem(int index, const QString &newName);


protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<TopicItem> m_topics;
    uint32_t m_next_id = 1;
};

#pragma once


#include "ui_states.hpp"
#include <QAbstractListModel>
#include <QObject>
#include <QtQml/qqml.h>
#include <unordered_map>


struct TopicItem {
    uint32_t id;
    QString name;
    bool pending;
};

class TopicListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Must be created by owner")

    //list state properties
    Q_PROPERTY(bool isAddingNewTopic READ isAddingNewTopic WRITE setIsAddingNewTopic
                   NOTIFY isAddingNewTopicChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY
                   currentIndexChanged)
    Q_PROPERTY(int editingIndex READ editingIndex WRITE setEditingIndex NOTIFY
                   editingIndexChanged)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        PendingRole,
        HoveredRole,
        FlagsRole,
    };
    explicit TopicListModel(QObject *parent = nullptr);

    //list interface override
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    Q_INVOKABLE void addFlags(int index, StateFlag flag);
    Q_INVOKABLE void removeFlags(int index, StateFlag flag);


    //API
    Q_INVOKABLE void addItem(const QString &name);
    Q_INVOKABLE void addConfirmedItem(uint32_t id, const QString &name);
    Q_INVOKABLE bool removeItem(int index);
    Q_INVOKABLE bool editItem(int index, const QString &newName);


    void deleteTopic(uint32_t id);
    void renameTopic(uint32_t id, const QString &newName);
    void confirmTopic(int index, uint32_t new_id);

    // list state API
    bool isAddingNewTopic() const { return m_isAddingNewTopic; }
    void setIsAddingNewTopic(bool value);

    int editingIndex() const { return m_editingIndex; }
    void setEditingIndex(int idx);

    int currentIndex() const { return m_currentIndex; }
    void setCurrentIndex(int idx);


signals:
    //state changes
    void isAddingNewTopicChanged();
    //emit the id of the item whos state is being set
    void editingIndexChanged(int idx);
    void currentIndexChanged(int idx);

    void requestAddTopic(int index, const QString &name);
    void deleteAddTopic(uint32_t id);


protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    //<-1 for not found
    int getTopicIndex(uint32_t id);

private:
    QVector<TopicItem> m_topics;
    std::unordered_map<uint32_t, ItemState> m_stateFlags;

    bool m_isAddingNewTopic = false;
    int m_editingIndex = -1;
    int m_currentIndex = -1;
};

#pragma once


#include "ui_state_manager.hpp"
#include <QAbstractListModel>
#include <QObject>
#include <QtQml/qqml.h>


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
    Q_PROPERTY(int hoveredId READ hoveredId WRITE setHoveredId NOTIFY hoveredIdChanged)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        PendingRole,
    };
    explicit TopicListModel(UIStateManager *stateManager, QObject *parent = nullptr);

    //list interface override
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

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

    int hoveredId() const { return m_hoveredId; }
    void setHoveredId(int id);


signals:
    //state changes
    void isAddingNewTopicChanged();
    //emit the id of the item whos state is being set
    void editingIndexChanged(int idx);
    void currentIndexChanged(int idx);
    void hoveredIdChanged(int id);

    //
    void requestAddTopic(int index, const QString &name);
    void deleteAddTopic(uint32_t id);


protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    //<-1 for not found
    int getTopicIndex(uint32_t id);

private:
    QVector<TopicItem> m_topics;
    UIStateManager *m_stateManager;

    bool m_isAddingNewTopic = false;
    int m_editingIndex = -1;
    int m_currentIndex = -1;
    int m_hoveredId = -1;
};

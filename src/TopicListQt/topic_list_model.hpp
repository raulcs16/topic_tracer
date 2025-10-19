#pragma once


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

    Q_PROPERTY(int editingIndex READ editingIndex WRITE setEditingIndex NOTIFY
                   editingIndexChanged)
    Q_PROPERTY(bool isAddingNewTopic READ isAddingNewTopic WRITE setIsAddingNewTopic
                   NOTIFY isAddingNewTopicChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY
                   currentIndexChanged)

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        PendingRole,
    };
    explicit TopicListModel(QObject *parent = nullptr);

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


    // State API
    int editingIndex() const { return m_editingIndex; }
    void setEditingIndex(int idx);
    bool isAddingNewTopic() const { return m_isAddingNewTopic; }
    void setIsAddingNewTopic(bool value);
    int currentIndex() const { return m_currentIndex; }
    void setCurrentIndex(int idx);

    void confirmTopic(int index, uint32_t new_id);

signals:
    void editingIndexChanged();
    void isAddingNewTopicChanged();
    void currentIndexChanged();
    void requestAddTopic(int index, const QString &name);
    void deleteAddTopic(uint32_t id);


protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<TopicItem> m_topics;


    int m_editingIndex = -1;
    bool m_isAddingNewTopic = false;
    int m_currentIndex = -1;
};

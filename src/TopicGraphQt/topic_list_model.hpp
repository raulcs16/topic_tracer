#pragma once


#include "topic_graph.hpp"
#include "topic_graph_store.hpp"
#include "ui_states.hpp"
#include <QAbstractListModel>
#include <QObject>
#include <QtQml/qqml.h>
#include <unordered_map>


class TopicListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Must be created by owner")
    //list state properties
    Q_PROPERTY(bool isAddingNewTopic READ isAddingNewTopic WRITE setIsAddingNewTopic
                   NOTIFY isAddingNewTopicChanged)
public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        LabelRole,
        FlagsRole,
    };
    explicit TopicListModel(TGStore *store, QObject *parent = nullptr);
    //list interface override
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    //API USER INTERACTIONS WITH LIST
    Q_INVOKABLE void setHovered(uint32_t id);
    Q_INVOKABLE void unsetHovered(uint32_t id);

    //single selection
    Q_INVOKABLE void selectIndex(int index);
    //multiple selections
    Q_INVOKABLE void toggleSelect(int index);
    Q_INVOKABLE void rangeSelect(int target);
    void clearSelection();

    //API
    // Q_INVOKABLE void addItem(const QString &name);
    // Q_INVOKABLE bool removeItem(int index);
    // Q_INVOKABLE bool editItem(int index, const QString &newName);
    bool isAddingNewTopic() const { return m_isAddingNewTopic; }
    void setIsAddingNewTopic(bool value);

    void clear();

signals:
    void isAddingNewTopicChanged();

public slots:
    //add or update a label
    void onLabelUpdated(uint32_t id);
    void onFlagUpdated(uint32_t id);


protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    int getIndex(uint32_t id);

private:
    QVector<uint32_t> m_ids;
    QMap<uint32_t, int> m_idToRow;
    TGStore *m_tgstore;
    std::vector<int> m_selectedIndexes;

    bool m_isAddingNewTopic = false;
    int m_lastSelectedIndex = -1;
    int m_rangeSelectedIndex = -1;
};

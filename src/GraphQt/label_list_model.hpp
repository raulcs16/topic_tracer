#pragma once


#include "graph.hpp"
#include "graph_store.hpp"
#include "ui_states.hpp"
#include <QAbstractListModel>
#include <QObject>
#include <QtQml/qqml.h>
#include <unordered_map>


class LabelListModel : public QAbstractListModel {
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
    explicit LabelListModel(GraphStore *store, QObject *parent = nullptr);
    //list interface override
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    //multiple selections
    std::vector<uint32_t> getIdInRange(uint32_t start, uint32_t target);

    bool isAddingNewTopic() const { return m_isAddingNewTopic; }
    void setIsAddingNewTopic(bool value);

    void clear();

signals:
    void isAddingNewTopicChanged();
    void hoverRequested(uint32_t id, bool isHovered);
    void selectRequested(uint32_t id);
    void toggleSelectionRequest(uint32_t id);
    void rangeSelectionRequest(uint32_t id);

public slots:
    //add or update a label
    void onLabelUpdated(uint32_t id);
    void onFlagUpdated(uint32_t id);
    void onClear();


protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    int getIndex(uint32_t id);

private:
    bool m_isAddingNewTopic = false;
    QVector<uint32_t> m_ids;
    QMap<uint32_t, int> m_idToRow;
    GraphStore *m_store;
};

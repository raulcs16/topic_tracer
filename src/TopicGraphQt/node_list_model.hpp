#pragma once


#include "ui_state_manager.hpp"

#include <QAbstractListModel>
#include <QObject>
#include <QtQml/qqml.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/GraphObserver.h>
#include <vector>


struct NodeItem {
    uint32_t id;
    QString label;
    double x;
    double y;
};

class NodeListModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Use TGC.nodes instead")

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        LabelRole,
        XRole,
        YRole,
        HighlightRole
    };

    explicit NodeListModel(UIStateManager *uiManger, QObject *parent = nullptr);

    //abstractlistmodel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    // bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    void resetNodes(const std::vector<NodeItem> &nodes);
public slots:
    void onGaphChanged();
    void onNodeStateChanged(const std::string &id, const StateFlags &flags);

private:
    int getNodeIndex(int id);

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    std::vector<NodeItem> m_nodes;
    UIStateManager *m_uiManager;
};
#pragma once

//https: //github.com/schulzch/qml-ogdf/blob/master/ogdfplugin/nodemodel.h


#include <QAbstractListModel>
#include <QObject>
#include <QtQml/qqml.h>
#include <ogdf/basic/GraphAttributes.h>
#include <ogdf/basic/GraphObserver.h>

class Graph;

class EdgeListModel : public QAbstractListModel, public ogdf::GraphObserver {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int count READ count NOTIFY countChanged);

public:
    enum Roles {
        SouceRole = Qt::UserRole + 1,
        SourceXRole,
        SourceYRole,
        TagertXRole,
        TargetYRole,
        BendsRole
    };

    EdgeListModel(Graph *graph, QObject *parent = nullptr);

    //abstractlistmodel interface
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    //graphobserver interface
    void cleared() override;
    void nodeAdded(ogdf::node v) override;
    void nodeDeleted(ogdf::node v) override;
    void edgeAdded(ogdf::edge e) override;
    void edgeDeleted(ogdf::edge e) override;

    //
    void attribuitesChanged();
    void reInit();

    //qml
    int count() const;
    Q_INVOKABLE QString get(int index) const;
    Q_INVOKABLE QString getSource(int index) const;
    Q_INVOKABLE QString getTarget(int index) const;
    Q_INVOKABLE void insert(const QString &edge,
                            const QString &source,
                            const QString &target);
    Q_INVOKABLE void remove(const QString &edge);
signals:
    void countChanged();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    Graph *m_gaph;
    QVector<ogdf::edge> m_edges;
};
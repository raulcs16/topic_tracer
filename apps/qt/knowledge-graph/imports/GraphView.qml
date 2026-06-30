pragma ComponentBehavior: Bound
import QtQuick

Item {
    id: root

    required property AbstractListModel nodeModel
    required property AbstractListModel edgeModel
    required property AbstractListModel boxModel

    property alias nodeDelegate: nodesList.delegate
    property alias edgeDelegate: edgeList.delegate
    property alias boxDelegate: boxList.delegate

    anchors.fill: parent

    Repeater {
        id: edgeList
        model: root.edgeModel
    }
    Repeater {
        id: nodesList
        model: root.nodeModel
    }
    Repeater {
        id: boxList
        model: root.boxModel
    }
}

pragma ComponentBehavior: Bound
import QtQuick

import TopicGraph

Item {
    id: root

    required property NodeListModel nodeModel
    required property EdgeListModel edgeModel

    property real viewWidth: 0
    property real viewHeight: 0

    width: viewWidth
    height: viewHeight

    Repeater {
        model: root.edgeModel
        delegate: EdgeSpline {
            viewWidth: root.viewWidth
            viewHeight: root.viewHeight
        }
    }

    Repeater {
        model: root.nodeModel
        delegate: NodeDelegate {
            viewWidth: root.viewWidth
            viewHeight: root.viewHeight
        }
    }
}

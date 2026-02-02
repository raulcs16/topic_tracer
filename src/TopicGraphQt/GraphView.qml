pragma ComponentBehavior: Bound
import QtQuick

import TopicGraph

Item {
    id: root

    required property NodeListModel nodeModel
    required property EdgeListModel edgeModel
    required property RectListModel rectModel

    property real viewWidth: 0
    property real viewHeight: 0

    width: viewWidth
    height: viewHeight

    Repeater {
        model: root.edgeModel
        delegate: EdgeSpline {}
    }

    Repeater {
        model: root.nodeModel
        delegate: NodeDelegate {
            model: root.nodeModel
        }
    }
    Repeater {
        model: root.rectModel
        delegate: RectDelegate {
            paddingX: 15
            paddingY: 25
        }
    }
}

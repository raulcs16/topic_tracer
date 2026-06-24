pragma ComponentBehavior: Bound
import QtQuick

Item {
    id: root

    required property AbstractListModel nodeModel
    property alias nodeDelegate: nodesList.delegate
    // required property NodeListModel nodeModel
    // required property EdgeListModel edgeModel
    // required property BoundingBoxListModel bboxModel

    width: parent.width
    height: parent.height

    // Repeater {
    //     model: root.edgeModel
    //     delegate: EdgeSpline {}
    // }
    Repeater {
        id: nodesList
        model: root.nodeModel
    }
    // Repeater {
    //     model: root.bboxModel
    //     delegate: BoundingBoxDelegate {}
    // }
}

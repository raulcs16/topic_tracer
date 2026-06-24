pragma ComponentBehavior: Bound
import QtQuick

Item {
    id: root

    required property AbstractListModel model
    property alias delegate: historyList.delegate

    ListView {
        id: historyList
        anchors.fill: parent
        clip: true
        model: root.model
    }
}

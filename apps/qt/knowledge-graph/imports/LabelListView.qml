pragma ComponentBehavior: Bound
import QtQuick

Item {
    id: root

    height: parent.height
    width: parent.width

    required property AbstractListModel model
    property alias delegate: list.delegate

    // Menu {
    //     id: listMenu
    //     background: MenuBackground {}
    //     MenuItemCard {
    //         text: qsTr("add topic")
    //         onTriggered: {
    //             root.model.isAddingNewTopic = true;
    //             topicView.contentY = topicView.contentHeight - topicView.height;
    //         }
    //     }
    // }

    // Menu {
    //     id: delegateMenu
    //     property int clickedIndex: -1
    //     background: MenuBackground {}

    //     MenuItemCard {
    //         text: qsTr("Rename...")
    //         onTriggered:
    //         // root.model.addFlags(delegateMenu.clickedIndex, ENUMS.StateFlag.EditMode);
    //         {}
    //     }
    //     MenuItemCard {
    //         text: qsTr("Delete")
    //         onTriggered:
    //         // root.model.removeItem(delegateMenu.clickedIndex);
    //         {}
    //     }
    // }
    Rectangle {
        anchors.fill: parent
        color: "black"
    }
    ListView {
        id: list
        anchors.fill: parent
        bottomMargin: 30
        model: root.model
        orientation: ListView.Vertical
        focus: true
    }

    // component MenuItemCard: MenuItem {
    //     id: rootItem
    //     background: Rectangle {
    //         color: "transparent"
    //         implicitWidth: 150
    //         implicitHeight: 30
    //     }
    //     contentItem: Text {
    //         text: rootItem.text
    //         font.pixelSize: 16
    //         font.weight: Font.DemiBold
    //         color: "white"
    //         verticalAlignment: Text.AlignVCenter
    //         leftPadding: 12
    //     }
    // }
    // component MenuBackground: Rectangle {
    //     radius: 6
    //     color: Colors.secondary
    //     border.color: Colors.accent
    //     border.width: 1
    //     implicitWidth: 200
    // }
}

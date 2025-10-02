pragma ComponentBehavior: Bound
import QtQuick 2.0

Flickable {
    id: view
    default property alias innerData: viewInner.data
    property int panStep: 15
    contentWidth: Math.max(width, viewInner.width)
    contentHeight: Math.max(height, viewInner.height)
    focus: true   // make sure this item can receive keyboard events

    Keys.onUpPressed: {
        contentY = Math.max(contentY - panStep, 0);
        console.log("Up");
    }
    Keys.onDownPressed: {
        contentY = Math.min(contentY + panStep, contentHeight - height);
        console.log("Down");
    }
    Keys.onLeftPressed: {
        contentX = Math.max(contentX - panStep, 0);

        console.log("Left");
    }
    Keys.onRightPressed: {
        contentX = Math.min(contentX + panStep, contentWidth - width);
        console.log("Right");
    }
    Behavior on contentX {
        NumberAnimation {
            duration: 45
        }
    }
    Behavior on contentY {
        NumberAnimation {
            duration: 45
        }
    }

    Item {
        id: viewInner
        x: childrenRect.width < view.width ? (view.width - childrenRect.width) / 2 : -childrenRect.x * 2
        y: childrenRect.height < view.height ? (view.height - childrenRect.height) / 2 : -childrenRect.y * 2
        width: childrenRect.width
        height: childrenRect.height
    }
    Component.onCompleted: {
        view.forceActiveFocus();
    }
}

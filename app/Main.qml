import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Basic

import Styles

ApplicationWindow {
    id: app
    width: 2074
    height: 1296
    title: qsTr("Application Window")
    visible: true

    RowLayout {
        anchors.fill: parent
        ColumnLayout {
            Rectangle {
                id: main_content
                color: Colors.primaryColor
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            Rectangle {
                id: status_bar
                color: Colors.accentColor
                Layout.fillWidth: true
                Layout.preferredHeight: 75
            }
        }
        Rectangle {
            id: side_bar
            color: Colors.secondaryColor
            Layout.preferredWidth: 300
            Layout.fillHeight: true
        }
    }
}

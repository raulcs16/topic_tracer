pragma Singleton

import QtQuick

QtObject {
    id: store
    property string suggestion: ""
    property ListModel historyList: ListModel {

        ListElement {
            text: "[71%] Built target Catch2WithMain"
            type: 0
        }

        ListElement {
            text: "Expected Token';'"
            type: 1
        }
    }
}

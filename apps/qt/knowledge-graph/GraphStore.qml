pragma Singleton

import QtQuick

QtObject {
    id: store

    property ListModel nodeList: ListModel {
        id: nodeList
    }

    Component.onCompleted: {
        nodeList.append({
            "itemId": 0,
            "label": "nodeA",
            "position": {
                "x": 10,
                "y": 10
            },
            "heatScore": 3.4,
            "selected": false,
            "editMode": false,
            "hovered": false,
            "hidden": false,
            "highlighted": false
        });
    }
}

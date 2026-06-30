pragma Singleton

import QtQuick

QtObject {
    id: store

    property ListModel nodeList: ListModel {
        id: nodeList
    }
    property ListModel edgeList: ListModel {
        id: edgeList
    }
    property ListModel boxList: ListModel {
        id: boxList
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
        edgeList.append({
            "source": {
                "x": 0,
                "y": 0
            },
            "target": {
                "x": 100,
                "y": 100
            },
            "bends": [
                {
                    "x": 0,
                    "y": 0
                },
                {
                    "x": 10,
                    "y": 10
                },
                {
                    "x": 30,
                    "y": 30
                },
                {
                    "x": 90,
                    "y": 90
                },
                {
                    "x": 100,
                    "y": 100
                },
            ],
            "lineWidth": 3,
            "dashed": true,
            "diamondTip": true,
            "hollowTip": false,
            "hovered": false,
            "highlighted": false,
            "hidden": false
        });
        boxList.append({
            "rect": {
                "x": -50,
                "y": -50,
                "width": 200,
                "height": 100
            },
            "label": "rectangle"
        });
    }
}

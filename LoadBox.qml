import QtQuick 2.9

Rectangle {
    property real value: 0

    width: 102
    height: 24
    border.color: "#000000"
    border.width: 1

    Rectangle {
        id: valueBox
        width: 0
        height: parent.height - 2
        anchors.left: parent.left
        anchors.leftMargin: 1
        anchors.verticalCenter: parent.verticalCenter
        color: "#00ff00"
    }

    Text {
        id: valueText
        text: "0%"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    onValueChanged: {
        valueBox.width = value
        valueText.text = value + "%"
    }
}

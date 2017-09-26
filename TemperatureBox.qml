import QtQuick 2.9

// CPU温度バー
Rectangle {
    property real value: 0

    width: 102
    height: 24
    border.color: "#000000"
    border.width: 1

    // 温度を示す緑色のバー
    Rectangle {
        id: valueBox
        width: 0
        height: parent.height - 2
        anchors.left: parent.left
        anchors.leftMargin: 1
        anchors.verticalCenter: parent.verticalCenter
        color: "#00ff00"
    }

    // 温度表示文字列
    Text {
        id: valueText
        text: qsTr("0℃")
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    // 負荷率値変更イベントハンドラ
    onValueChanged: {
        valueBox.width = value
        valueText.text = value + qsTr("℃")
    }
}

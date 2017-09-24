import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: root
    visible: true
    width: column.x + column.width
    minimumWidth: column.x + column.width
    maximumWidth: column.x + column.width
    height: column.y + column.height
    minimumHeight: column.y + column.height
    maximumHeight: column.y + column.height
    title: qsTr("CPU Load")
    onClosing: timer.stop

    Connections {
        target: cpuLoad
        onLoadUpdated: {
            var box = repeaterCpuLoad.itemAt(nIndex).children[1]
            box.value = Math.round(nLoad)
        }
    }

    Timer {
        id: timer
        interval: 1000
        running: true
        repeat: true
        onTriggered: {
            cpuLoad.startGetCpuLoad()
        }
    }

    Column {
        id: column
        spacing: 2

        Repeater {
            id: repeaterCpuLoad
            objectName: "cpuLoadCpu"

            Row {
                Text {
                    width: 64
                    anchors.verticalCenter: parent.verticalCenter
                    text: index == 0 ? "Overall: " : "CPU" + index + ": "
                }

                LoadBox {
                }
            }
        }
    }

    Menu {
        id: menu
        width: item1.width
        height: item1.height
        background: Rectangle { opacity: 0 }

        MenuItem {
            id: item1
            width: 100
            background: Rectangle { anchors.fill: parent; color: "#ff0000"; radius: 5 }
            text: qsTr("Quit")
            onTriggered: {
                Qt.quit()
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        property int pressButton: 0

        onPressed: {
            pressButton = pressedButtons
        }

        onDoubleClicked: {
            if (pressedButtons & Qt.LeftButton)
                Qt.quit()
        }

        onClicked: {
            if (pressButton & Qt.RightButton) {
                menu.x = mouseX; menu.y = mouseY
                menu.open()
            }
        }
    }
}

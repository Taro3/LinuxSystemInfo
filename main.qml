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
//            var loads = cpuLoad.cpuLoad()
//            console.log(loads)
//            for (var i = 0; i < cpuLoad.cpuCount() + 1; ++i) {
//                var box = repeaterCpuLoad.itemAt(i).children[1]
//                box.value = Math.round(loads[i])
//            }
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
//        property point dragOffset: "0, 0"
        property int pressButton: 0

        onPressed: {
            pressButton = pressedButtons
//            if (pressedButtons & Qt.LeftButton)
//                dragOffset = Qt.point(mouseX, mouseY)
        }

//        onPositionChanged: {
//            if (pressedButtons & Qt.LeftButton) {
//                var pos = mapToGlobal(mouseX, mouseY)
//                root.x = pos.x - dragOffset.x
//                root.y = pos.y - dragOffset.y
//            }
//        }

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

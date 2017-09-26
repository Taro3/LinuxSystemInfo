import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

ApplicationWindow {
    id: root
    visible: true
    width: row.x + row.width
    minimumWidth: row.x + row.width
    maximumWidth: row.x + row.width
    height: row.y + row.height
    minimumHeight: row.y + row.height
    maximumHeight: row.y + row.height
    title: qsTr("Sys Info")
    onClosing: timer.stop

    Connections {
        target: cpuLoad
        onLoadUpdated: {
            var box = repeaterCpuLoad.itemAt(nIndex).children[1]
            box.value = Math.round(nLoad)
        }
    }

    Connections {
        target: cpuTemperature
        onTemperatureUpdated: {
            var box = repeaterCpuTemerate.itemAt(nCpuIndex).children[1]
            box.value = nTemperature
        }
    }


    Timer {
        id: timer
        interval: 1000
        running: true
        repeat: true
        onTriggered: {
            cpuLoad.startGetCpuLoad()
            cpuTemperature.startGetCpuTemperature()
        }
    }

    Row {
        id: row
        spacing: 2

        Column {
            id: columnCpuLoad
            spacing: 2

            Text {
                text: qsTr("CPU Load")
            }

            Repeater {
                id: repeaterCpuLoad
                objectName: "cpuLoadCpu"

                Row {
                    Text {
                        width: 64
                        anchors.verticalCenter: parent.verticalCenter
                        text: index == 0 ? qsTr("Overall: ") : qsTr("CPU") + (index - 1) + qsTr(": ")
                    }

                    LoadBox {
                    }
                }
            }
        }

        Column {
            id: columnCpuTemperature
            spacing: 2

            Text {
                text: qsTr("CPU Temperature")
            }

            Repeater {
                id: repeaterCpuTemerate
                objectName: "cpuTemperatureCpu"

                Row {
                    Text {
                        width: 64
                        anchors.verticalCenter: parent.verticalCenter
                        text: index == 0 ? qsTr("Overall: ") : qsTr("Core") + (index - 1) + qsTr(": ")
                    }

                    TemperatureBox {
                    }
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

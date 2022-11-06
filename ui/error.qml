import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id: errorWindow
    x: Screen.width / 2 - width / 2
    y: Screen.height / 2 - height / 2
    maximumHeight: height
    maximumWidth: width
    minimumHeight: height
    minimumWidth: width
    width: 250
    height: 150
    visible: true
    title: qsTr("Error")

    Rectangle
    {
        anchors.fill: parent
        Text {
            anchors.centerIn: parent
            width: 200
            height: 30
            id: errorText
            font.pixelSize: 18
            text: uiController.uiError
        }
        Rectangle {
            anchors.horizontalCenter: errorText.horizontalCenter
            anchors.top: errorText.bottom
            anchors.bottomMargin: 10
            width: 40
            height: 30
            color: "yellow"
            Text {
                anchors.centerIn: parent
                id: buttonText
                text: qsTr("Ok")
            }
            MouseArea {
                anchors.fill: parent
                onClicked: errorWindow.close()
            }
        }
    }
}

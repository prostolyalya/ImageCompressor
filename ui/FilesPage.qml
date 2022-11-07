import QtQuick 2.12

Item {
    anchors.fill: parent
    Text {
        id: path
        text: uiController.uiPath
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 7
        font.pixelSize: 18
        height: 20
    }
    Rectangle
    {
        id: clientFiles
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: path.height + 15
        width: parent.width
        height: parent.height - 30
        color: "#D9DDDC"
        border.width: 1
        border.color: "black"

        ListView
        {
            anchors.fill: parent
            model: uiModel
            clip: true
            focus: true
            delegate: Rectangle {
                width: parent.width
                height: 50
                border.color: "black"
                color: "#95C8D8"
                Text {
                    id: textElemName
                    text: name
                    clip: true
                    font.pixelSize: 18
                    anchors.left: parent.left
                    anchors.leftMargin: 45
                    anchors.verticalCenter: parent.verticalCenter
                    width: parent.width / 2
                    color: "#311432"
                }
                Image {
                    id: icon
                    anchors.left: parent.left
                    width: 30
                    height: 30
                    anchors.leftMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    source: type == 0 ? "qrc:/images/folder" : type == 1 ? "qrc:/images/image" : "qrc:/images/zip"
                }
                Text {
                    id: sizeElem
                    text: size
                    visible: type != 0
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.rightMargin: 20
                }
                AnimatedImage {
                    id: gif
                    source: "qrc:/images/process"
                    anchors.right: sizeElem.left
                    anchors.rightMargin: 30
                    anchors.verticalCenter: parent.verticalCenter
                    width: 60
                    height: 60
                    paused: !working
                    visible: working
                }
                MouseArea
                {
                    anchors.fill: parent
                    onDoubleClicked:
                    {
                        if(type != 0)
                        {
                            gif.visible = true
                            gif.paused = false
                        }
                        uiController.change(name)
                    }
                }
            }
        }
    }

}

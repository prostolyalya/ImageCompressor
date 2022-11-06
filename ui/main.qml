import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    width: 800
    height: 600
    minimumHeight: 480
    minimumWidth: 640
    x: Screen.width / 2 - width / 2
    y: Screen.height / 2 - height / 2
    visible: true
    title: qsTr("BMP Image compressor")

    FilesPage {
        anchors.fill: parent
    }
}

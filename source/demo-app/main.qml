import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("RSS Feed Viewer using Node.js")
    onClosing: Qt.quit()

    ListView {
        anchors.fill: parent
        model: rssFeed.entries

        delegate: Text {
            height: 50
            text: modelData
        }
    }
}

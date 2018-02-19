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
        anchors.topMargin: 15
        model: rssFeed.entries

        delegate: Item {
            width: parent.width
            height: heading.contentHeight + content.contentHeight + 50
            Rectangle {
                id: card
                anchors.fill: parent
                anchors.leftMargin: 15
                anchors.rightMargin: 15
                anchors.bottomMargin: 15
                radius: 15
                color: "#eee"

                Text {
                    id: heading
                    anchors.top: parent.top
                    anchors.topMargin: 10
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    height: 30
                    text: modelData.split("\n")[0] || "Heading missing"
                    font.pointSize: 15
                    font.bold: true
                    elide: Text.ElideRight
                }

                Text {
                    id: content
                    anchors.top: heading.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.bottom: parent.bottom
                    text: modelData.split("\n")[1] || "Content missing"
                    font.pointSize: 14
                    wrapMode: Text.Wrap
                }
            }
        }
    }
}

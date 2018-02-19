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
        anchors.topMargin: 10
        model: rssFeed.entries

        delegate: Item {
            width: parent.width
            height: heading.contentHeight + content.contentHeight + 30
            Rectangle {
                id: card
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                anchors.bottomMargin: 10
                radius: 10
                color: "#eee"

                Text {
                    id: heading
                    anchors.top: parent.top
                    anchors.topMargin: 6
                    anchors.left: parent.left
                    anchors.leftMargin: 6
                    anchors.right: parent.right
                    anchors.rightMargin: 6
                    height: 20
                    text: modelData.split("\n")[0] || "Heading missing"
                    font.pointSize: 12
                    font.bold: true
                    elide: Text.ElideRight
                }

                Text {
                    id: content
                    anchors.top: heading.bottom
                    anchors.left: parent.left
                    anchors.leftMargin: 6
                    anchors.right: parent.right
                    anchors.rightMargin: 6
                    anchors.bottom: parent.bottom
                    text: modelData.split("\n")[1] || "Content missing"
                    font.pointSize: 10
                    wrapMode: Text.Wrap
                }
            }
        }
    }
}

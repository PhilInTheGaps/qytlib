import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("qytlib demo")

    Column {
        id: column
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        Item {
            anchors.left: parent.left
            anchors.right: parent.right

            height: text_field.height

            TextField {
                id: text_field

                anchors.left: parent.left
                anchors.right: load_button.left
                anchors.rightMargin: 5

                placeholderText: "Video ID or URL"
                selectByMouse: true
            }

            Button {
                id: load_button

                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                text: "Load"

                onClicked: {
                    busy.visible = true
                    demo_client.loadVideo(text_field.text)
                }
            }
        }

        Label {
            text: "Video information:"
        }

        Grid {
            id: grid
            anchors.left: parent.left
            anchors.right: parent.right
            columns: 2
            columnSpacing: 10

            Label {
                text: "Title:"
                visible: demo_client.title.length
            }

            Label {
                text: demo_client.title
            }

            Label {
                text: "Author:"
                visible: demo_client.author.length
            }

            Label {
                text: demo_client.author
            }

            Label {
                text: "Description:"
                visible: demo_client.description.length
            }

            Label {
                text: demo_client.description
                wrapMode: Text.WordWrap
            }
        }
    }

    BusyIndicator {
        id: busy
        visible: false
        anchors.centerIn: column

        Connections {
            target: demo_client
            onVideoInfoChanged: busy.visible = false
        }
    }
}

import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtMultimedia 5.2

Window {
    visible: true
    width: 700
    height: 700
    title: qsTr("qytlib demo")

    Connections {
        target: demo_client
        onStreamChanged: {
            video.play()
            busy.visible = false
        }
    }

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

                text: "https://www.youtube.com/watch?v=7Gr63DiEUxw"
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

        Rectangle {
            anchors.left: parent.left
            anchors.right: parent.right
            height: width * (9 / 16)
            color: "black"

            Video {
                id: video

                property bool isPlaying: false

                anchors.fill: parent

                onPlaying: isPlaying = true
                onPaused: isPlaying = false
                onStopped: isPlaying = false

                source: demo_client.stream

                volume: volume_slider.value

                function playPause() {
                    if (video.isPlaying) {
                        video.pause()
                    } else {
                        video.play()
                    }
                }

                MouseArea {
                    id: video_area

                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.bottom: video_controls.top

                    hoverEnabled: true

                    onClicked: {
                        video.playPause()
                    }
                }

                MouseArea {
                    id: video_controls_area

                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    height: 50

                    hoverEnabled: true
                    acceptedButtons: Qt.NoButton
                }

                Rectangle {
                    id: video_controls

                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    height: 50

                    visible: (video_area.containsMouse
                              || video_controls_area.containsMouse)
                             && demo_client.stream.length && !busy.visible

                    color: "black"

                    Label {
                        id: play_pause

                        anchors.left: parent.left
                        anchors.leftMargin: 5
                        anchors.top: parent.top

                        text: video.isPlaying ? "\u23F8" : "\u23F5"
                        color: "white"
                        font.pixelSize: 35
                        style: Text.Outline

                        MouseArea {
                            anchors.fill: parent
                            onClicked: video.playPause()
                        }
                    }

                    Slider {
                        id: volume_slider

                        anchors.left: play_pause.right
                        anchors.verticalCenter: parent.verticalCenter

                        from: 0
                        to: 1
                        value: 0.25
                    }
                }
            }

            BusyIndicator {
                id: busy
                visible: false
                anchors.centerIn: parent
            }

            Label {
                anchors.centerIn: parent
                color: "white"
                visible: !demo_client.stream.length && !busy.visible

                text: "No video loaded."
            }
        }

        Label {
            anchors.left: parent.left
            anchors.right: parent.right

            text: demo_client.video ? demo_client.video.title : ""
            font.pointSize: 14
            font.bold: true

            clip: true
            elide: Text.ElideRight
        }

        Label {
            text: demo_client.video ? demo_client.video.author : ""
            font.bold: true

            clip: true
            elide: Text.ElideRight
        }

        Flickable {
            anchors.left: parent.left
            anchors.right: parent.right
            height: 200

            contentHeight: description_label.height
            clip: true

            ScrollBar.vertical: ScrollBar {}

            Label {
                id: description_label

                anchors.left: parent.left
                anchors.right: parent.right

                text: demo_client.video ? demo_client.video.description : ""
                wrapMode: Text.WordWrap
            }
        }
    }
}

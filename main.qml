import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Item {
    /*Button {
        text: "A button"
        style: ButtonStyle {
            background: Rectangle {
                implicitWidth: 100
                implicitHeight: 25
                border.width: control.activeFocus ? 2 : 1
                border.color: "#888"
                radius: 4
                gradient: Gradient {
                    GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#eee" }
                    GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
                }
            }
        }
    }*/
    Text {
        id: text
        color: "#a6a6a6"
        text: "Test project. QtQuick2 and OpenGL 4.3 Core."
        anchors.horizontalCenter: parent.horizontalCenter;
        anchors.bottom: parent.bottom
        anchors.topMargin: 10
    }

    Rectangle {
        id: rect
        width: 50
        height: 50
        x: 100
        y: 100
        color: "#232454"
        opacity: 0.5

        Behavior on x { SpringAnimation { spring: 10; damping: 0.2 } }
        Behavior on y { SpringAnimation { spring: 10; damping: 0.2 } }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            rect.x = mouse.x - rect.width/2
            rect.y = mouse.y - rect.height/2
        }
    }
    Button {
        id: button1
        text: "Button 1"
        tooltip:"This is an interesting tool tip"
        //Layout.fillWidth: true
    }
}

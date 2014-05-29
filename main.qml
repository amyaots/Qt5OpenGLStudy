import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import QtQuick.XmlListModel 2.0

Item {
id: root
    Text {
        objectName: "text1"
        id: text1
        color: "#a6a6a6"
        text: qsTr("EasyChemystry. Myaots Aleksandr 2014")
        anchors.horizontalCenter: parent.horizontalCenter;
        anchors.bottom: parent.bottom
        anchors.topMargin: 10
    }
    XmlListModel {
        id: elementsModel
        source: "elements.xml"
        query: "/elements/atom"
        XmlRole { name: "names"; query: "name/string()" }
        XmlRole { name: "symbols"; query: "symbol/string()" }
        XmlRole { name: "atomicmass"; query: "atomicMass/number()" }
        XmlRole { name: "atomicnum"; query: "atomicNum/number()" }
        XmlRole { name: "radiusx"; query: "radius/number()" }
        XmlRole { name: "elecronegat"; query: "elecronegativity/number()" }
        XmlRole { name: "groupx"; query: "group/string()" }
        XmlRole { name: "categoryx"; query: "category/string()" }
        XmlRole { name: "metalx"; query: "metal/number()" }
        XmlRole { name: "xrayx"; query: "xray/number()" }
    }
    Text {
        id: testtext
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        text: elementsModel.count
        color: "white"
    }

    GridView {
        id: gridv
        visible: button1.checked
        anchors.top: button1.bottom
        anchors.topMargin: 20
        anchors.left: button1.left
        width: 950; height: 400
        model: elementsModel
        cellWidth: 93
        cellHeight: 53
        //spacing: 2
        delegate:
            Cell{
                name: qsTr(names)
                symbol: symbols
                atomnumber: atomicnum
                group: groupx
                category: categoryx
                electr: elecronegat
                radiusr: radiusx
                atommass: atomicmass
                metal: metalx
                xray: xrayx
                visible: (names=="0")? false:true
                MouseArea {
                    anchors.fill: parent
                    onClicked: testtext.text=parent.symbol
                }
            }
    }

    Text
    {
        property int fpsDraw: windowq.fps
        id: fps
        color: "white"
        text: "FPS: " + fpsDraw
    }

    /*Rectangle {
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
    }*/
    Button {
        id: button1
        //tooltip: qsTr("Таблица Менделеева")
        checkable: true
        anchors{
            left: parent.left
            leftMargin: 50
            top: parent.top
            topMargin: 20
            //verticalCenter: parent.verticalCenter
            //verticalCenterOffset: -100
        }
        style: ButtonStyle {
        background: Rectangle {
                implicitWidth: 130
                implicitHeight: 45
                border.color: Qt.lighter(color)
                color: control.pressed ? "#00886d" : "#00d1b6"
                /*gradient: Gradient {
                    GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#eee" }
                    GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
                }*/

                Text {
                    text: qsTr("Таблица\n Менделеева")
                    font.family: "Arial"
                    font.pointSize: 11
                    color: "white"
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    }
            }
        }
    }
    Button {
        id: button2
        anchors.left: button1.right
        anchors.leftMargin: 20
        //anchors.verticalCenter: parent.verticalCenter
        anchors.top: parent.top
        anchors.topMargin: 20
        style: ButtonStyle {
        background: Rectangle {
                implicitWidth: 130
                implicitHeight: 45
                //border.width: control.activeFocus ? 2 : 1
                border.color: Qt.lighter(color)
                color: control.pressed ? "#00886d" : "#00d1b6"
                Text {
                    text: qsTr("Таблица\n растворимости")
                    font.family: "Arial"
                    font.pointSize: 11
                    color: "white"
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    }
            }
        }
    }
    /*Button {
        id: button1
        text: "Button 1"
        tooltip:"This is an interesting tool tip"
        //Layout.fillWidth: true
    }*/
}

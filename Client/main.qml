import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    Button {
        x: 36
        y: 35
        text: "Start"
        onClicked: app.startSending()
    }

    Button {
        x: 164
        y: 35
        text: "Stop"
        onClicked: app.stopSending()
    }

    Button {
        x: 36
        y: 89
        text: "Interval"
        onClicked: app.setInterval(intervalInput.value)
    }

    Button {
        x: 36
        y: 146
        text: "Toggle lights"
        onClicked: app.toggleLights()
    }

    SpinBox {
        x: 164
        y: 89
        editable: true
        to: 10000
        objectName: "intervalInput"
        value: 1000
        id: intervalInput

    }

    Rectangle {
        id: lightsIndicator
        x: 153
        y: 156
        width: 20
        height: 20
        color: app.lights ? "green" : "red";
    }
}

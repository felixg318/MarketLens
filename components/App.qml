import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MarketLens 1.0

ApplicationWindow {
    id: root
    width: 1920
    height: 1080
    minimumWidth: 800
    minimumHeight: 600
    visible: true
    title: qsTr("MarketLens")
    color: Profile.colors.windowBg

    Loader {
        id: mainLoader
        anchors.fill: parent
        source: "Lockscreen.qml"
    }

}

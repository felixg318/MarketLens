import QtQuick
import QtQuick.Controls
import MarketLens 1.0

Dialog {
    property alias message: messageLabel.text

    modal: true
    padding: 0

    background: Rectangle {
        color: Profile.colors.panelBg
        border.width: 1
        border.color: Profile.colors.panelBorder
    }

    header: Rectangle {
        color: Profile.colors.headerBg
        implicitHeight: 40

        Label {
            text: parent.parent.title
            color: Profile.colors.textColor
            font.bold: true
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 12
        }
    }

    contentItem: Label {
        id: messageLabel
        color: Profile.colors.textColor
        leftPadding: 12
        topPadding: 12
        rightPadding: 12
        bottomPadding: 12
        wrapMode: Text.Wrap

        background: Rectangle {
            color: Profile.colors.headerBg
        }
    }

    footer: Rectangle {
        color: Profile.colors.headerBg
        implicitHeight: 44

        Button {
            text: qsTr("OK")
            anchors.right: parent.right
            anchors.rightMargin: 12
            anchors.verticalCenter: parent.verticalCenter

            background: Rectangle {
                color: Profile.colors.headerBg
                radius: 4
            }

            contentItem: Text {
                text: parent.text
                color: Profile.colors.textColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }

            onClicked: close()
        }
    }
}

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MarketLens 1.0

Rectangle {
    id: root
    color: Profile.colors.windowBg

    Rectangle {
        width: 360
        height: 300
        radius: 16
        color: Profile.colors.surfaceBg
        border.color: Profile.colors.panelBorder
        border.width: 2
        anchors.centerIn: parent

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 28
            spacing: 12

            Text {
                text: qsTr("Please select an API Provider")
                font.pixelSize: 18
                color: Profile.colors.textColor
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
            }

            ComboBox {
                id: providerComboBox
                currentIndex: -1
                model: ListModel {
                    ListElement { key: "Alpha Vantage (FREE)" }
                }
                font.pixelSize: 16
                Layout.fillWidth: true

                background: Rectangle {
                    color: Profile.colors.headerBg
                    radius: 4
                }

                contentItem: Text {
                    leftPadding: 12
                    rightPadding: parent.indicator.width + 12
                    text: parent.displayText
                    color: Profile.colors.textColor
                    verticalAlignment: Text.AlignVCenter
                    elide: Text.ElideRight
                }

                delegate: ItemDelegate {
                    width: parent.width
                    text: key
                    contentItem: Text {
                        text: parent.text
                        color: Profile.colors.textColor
                        verticalAlignment: Text.AlignVCenter
                        leftPadding: 12
                    }
                    background: Rectangle {
                        color: highlighted ? Profile.colors.itemBg : Profile.colors.surfaceBg
                    }
                }

                onCurrentIndexChanged: {
                    Profile.setProvider(currentIndex)
                    if (Profile.keyExists(currentIndex))
                        mainLoader.source = "MainView.qml"
                }
            }

            Text {
                text: qsTr("Enter your API Key")
                font.pixelSize: 18
                color: Profile.colors.textColor
                horizontalAlignment: Text.AlignHCenter
                Layout.alignment: Qt.AlignHCenter
            }

            TextField {
                id: apiKeyField
                placeholderText: qsTr("API Key")
                text: Profile.key
                font.pixelSize: 16
                color: Profile.colors.textColor
                Layout.fillWidth: true

                background: Rectangle {
                    color: Profile.colors.headerBg
                    radius: 4
                }

                onTextChanged: Profile.setKey(text)
            }

            Text {
                id: submitErrorText
                text: ""
                color: "red"
                font.pixelSize: 12
                Layout.fillWidth: true
                horizontalAlignment: Text.AlignHCenter
            }

            Button {
                text: qsTr("Submit")
                font.pixelSize: 16
                Layout.fillWidth: true
                height: 38
                enabled: apiKeyField.text.trim().length > 0 && providerComboBox.currentIndex >= 0

                background: Rectangle {
                    color: Profile.colors.headerBg
                    radius: 4
                }

                contentItem: Text {
                    text: parent.text
                    color: parent.enabled ? Profile.colors.textColor : Profile.colors.secondaryText
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    submitErrorText.text = ""
                    if (Profile.validate())
                        mainLoader.source = "MainView.qml"
                    else
                        submitErrorText.text = qsTr("API key validation failed. Please check and try again.")
                }
            }
        }
    }
}

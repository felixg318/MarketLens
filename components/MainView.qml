import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import MarketLens 1.0
  
Rectangle {
    id: appWindow
    color: Profile.colors.windowBg
    anchors.fill: parent

    //future: add a historical search
    Rectangle {
        id: searchBar
        color: Profile.colors.surfaceBg
        width: parent.width
        height: 60
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        RowLayout {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 12

            TextField {
                id: searchField
                text: MarketData.lookupSymbol
                placeholderText: "Search..."
                Layout.fillWidth: true
                height: parent.height * 0.7
                color: Profile.colors.textColor

                background: Rectangle {
                    color: Profile.colors.headerBg
                    radius: 4
                }

                Timer {
                    id: debounceTimer
                    interval: 200
                    repeat: false
                    onTriggered: MarketData.setLookupSymbol(searchField.text)
                }

                onTextEdited: debounceTimer.restart()
                onTextChanged: {
                    if (text.length > 0) {
                        filterPopup.open()
                    } else {
                        filterPopup.close()
                    }
                }
            }

            Button {
                text: Profile.theme === 1 ? "Light" : "Dark"
                onClicked: Profile.setTheme(Profile.theme === 1 ? 0 : 1)

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
            }
        }
    }

    Popup {
        id: filterPopup
        modal: false
        focus: false
        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
        x: searchBar.x
        y: searchBar.height
        width: searchBar.width
        height: Math.min(400, parent.height * 0.6)

        background: Rectangle {
            color: Profile.colors.popupBg
            border.color: Profile.colors.borderColor
            radius: 6
        }

        ListView {
            anchors.fill: parent
            focus: true
            model: MarketData.symbolFilterList
            spacing: 2
            clip: true

            delegate: Item {
                width: ListView.view.width
                height: 36

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor

                    onClicked: {
                        MarketData.setUserSymbol(modelData.ticker);
                        searchField.text = "";
                        filterPopup.close();
                    }
                }

                Rectangle {
                    anchors.fill: parent
                    color: mouseArea.pressed ? Profile.colors.pressedBg : mouseArea.containsMouse ? Profile.colors.itemBg : "transparent"

                      Text {
                           anchors.fill: parent
                           anchors.leftMargin: 12
                           verticalAlignment: Text.AlignVCenter
                           color: Profile.colors.textColor
                           text: modelData.ticker + " - " + modelData.name
                           font.pixelSize: 14
                      }
                 }
             }

        }
        onClosed: {
            searchField.text = ""
        }
     }

    Rectangle {
        id: tabsBar
        color: Profile.colors.surfaceBg
        width: parent.width
        height: tabs.implicitHeight
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: searchBar.bottom

        TabBar {
            id: tabs
            anchors.fill: parent
            background: Rectangle {
                color: Profile.colors.surfaceBg
            }

            TabButton {
                id: chartTab
                text: qsTr("Chart")
                background: Rectangle {
                    color: chartTab.checked ? Profile.colors.checkedBg : "transparent"
                }
                contentItem: Text {
                    text: chartTab.text
                    color: Profile.colors.textColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
    }

    //future: change to loader for optimized rendering
    StackLayout {
        id: tabContent
        anchors.top: tabsBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        currentIndex: tabs.currentIndex

        ChartTab { }
    }
}

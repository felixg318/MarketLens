import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts
import MarketLens 1.0
import MarketLens 1.0 as MarketLens

Item {
    Layout.fillWidth: true
    Layout.fillHeight: true
    property bool showTable: false
    property string exportFormat: ""

    Rectangle {
        id: chartPanel
        color: Profile.colors.panelBg
        anchors.fill: parent
        anchors.margins: 12

        ColumnLayout {
            anchors.fill: parent

            RowLayout {
                Layout.fillWidth: true

                Rectangle {
                    id: symbolInfo
                    Layout.fillWidth: true
                    height: 28
                    color: Profile.colors.headerBg

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        text: MarketData.userSymbol + " " + MarketData.interval
                        color: Profile.colors.textColor
                    }
                }

                Button {
                    id: viewToggle
                    text: showTable ? qsTr("Chart") : qsTr("Table")
                    onClicked: showTable = !showTable

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
            //custom chart in ../src/ui/candlestickchart.h
            MarketLens.CandlestickChart {
                id: chartView
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: !showTable
                model: MarketData.ohlcvModel
                profile: Profile
                candleWidth: 8
                spacing: 2
                upColor: "#26a69a"
                downColor: "#ef5350"
            }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true
                visible: showTable

                ToolBar {
                    id: myToolbar
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 30

                    background: Rectangle {
                        color: Profile.colors.headerBg
                    }

                    RowLayout {
                        anchors.margins: 5
                        spacing: 10

                        ToolButton {
                            background: Rectangle {
                                color: parent.hovered ? Profile.colors.itemBg : "transparent"
                            }
                            palette.buttonText: Profile.colors.textColor
                            text: "Export as CSV"

                            onClicked: {
                                if (MarketData.userSymbol === "") {
                                    noSymbolDialog.open()
                                    return
                                }
                                exportFormat = "csv";
                                fileDialog.open()
                            }
                        }


                        ToolButton {
                            background: Rectangle {
                                color: parent.hovered ? Profile.colors.itemBg : "transparent"
                            }
                            palette.buttonText: Profile.colors.textColor
                            text: "Export as JSON"

                            onClicked: {
                                if (MarketData.userSymbol === "") {
                                    noSymbolDialog.open()
                                    return
                                }
                                exportFormat = "json";
                                fileDialog.open()
                            }
                        }
                    }

                    FileDialog {
                        id: fileDialog
                        fileMode: FileDialog.SaveFile
                        currentFolder: StandardPaths.writableLocation(StandardPaths.DocumentsLocation)
                        selectedFile: currentFolder + "/" + MarketData.userSymbol + "_" + MarketData.interval +  "." + exportFormat

                        onAccepted: {
                            if (exportFormat === "csv")
                                MarketData.ohlcvModel.exportToCsv(selectedFile)
                            else
                                MarketData.ohlcvModel.exportToJson(selectedFile)
                        }
                    }

                    ThemedDialog {
                        id: noSymbolDialog
                        title: qsTr("No Symbol Selected")
                        message: qsTr("Please select a stock symbol before exporting data.")
                    }
                }

                Row {
                    id: headerRow
                    anchors.topMargin: 6
                    anchors.top: myToolbar.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    height: 30
                    z: 2

                    Repeater {
                        model: ["Date", "Open", "High", "Low", "Close", "Volume"]

                        Rectangle {
                            width: headerRow.width / 6
                            height: 30
                            color: Profile.colors.headerBg
                            border.width: 1
                            border.color: Profile.colors.headerBorder

                            Text {
                                anchors.centerIn: parent
                                color: Profile.colors.textColor
                                text: modelData
                                font.pixelSize: 13
                            }
                        }
                    }

                }

                TableView {
                    id: tableView
                    anchors.top: headerRow.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    clip: true
                    model: MarketData.ohlcvModel

                    delegate: Rectangle {
                        color: Profile.colors.itemBg
                        border.width: 1
                        border.color: Profile.colors.headerBg

                        Text {
                            anchors.fill: parent
                            anchors.leftMargin: 8
                            verticalAlignment: Text.AlignVCenter
                            color: Profile.colors.textColor
                            text: model.display
                            font.pixelSize: 13
                            elide: Text.ElideRight
                        }
                    }
                    flickableDirection: Flickable.VerticalFlick
                    rowHeightProvider: function(row) { return 28; }
                    columnWidthProvider: function(col) { return (tableView.width) / 6; }
                }
            }
        }
    }
}

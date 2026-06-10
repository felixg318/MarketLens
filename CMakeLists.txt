cmake_minimum_required(VERSION 3.16)

project(MarketLens VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 REQUIRED COMPONENTS Charts Core Gui Qml Quick QuickControls2 Network Sql Widgets)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(MarketLens)
set_target_properties(MarketLens PROPERTIES RUNTIME_OUTPUT_NAME "marketlens")

target_compile_definitions(MarketLens PRIVATE QT_QML_DEBUG)

target_link_libraries(MarketLens PRIVATE 
	Qt6::Charts
	Qt6::Core
	Qt6::Gui
	Qt6::Qml
	Qt6::Quick
	Qt6::QuickControls2
	Qt6::Network
	Qt6::Sql
	Qt6::Widgets
)

target_include_directories(MarketLens PUBLIC 
	src
	src/core
	src/api
	src/data
	src/ui
)

qt_add_qml_module(
	MarketLens
	URI MarketLens
	VERSION 1.0
	QML_FILES
	        components/App.qml
		components/ChartTab.qml
		components/Lockscreen.qml
	        components/MainView.qml
		components/ThemedDialog.qml
	SOURCES
	        src/core/profile.h
		src/core/profile.cpp
		src/core/themecolors.h
		src/core/themecolors.cpp
		src/core/marketdata.h
		src/core/marketdata.cpp
	        src/api/apihandler.h
		src/api/alphavantagehandler.h
		src/api/alphavantagehandler.cpp
		src/api/networkmanager.h
		src/api/networkmanager.cpp
		src/data/ohlcv.h
		src/data/dbmanager.h
		src/data/dbmanager.cpp
		src/data/ohlcvmodel.h
		src/data/ohlcvmodel.cpp
		src/ui/candlestickchart.h
		src/ui/candlestickchart.cpp
	        src/main.cpp
)

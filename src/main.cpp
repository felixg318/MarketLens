#include <iostream>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <qqml.h>

#include <marketdata.h>
#include <profile.h>
#include <themecolors.h>
#include <candlestickchart.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("MarketLens");
    QCoreApplication::setApplicationName("MarketLens");
    QQmlApplicationEngine engine;

    Profile profile(&app);
    MarketData marketData(&profile, &app);

    qmlRegisterSingletonInstance("MarketLens", 1, 0, "Profile", &profile);
    qmlRegisterSingletonInstance("MarketLens", 1, 0, "MarketData", &marketData);
    qmlRegisterType<CandlestickChart>("MarketLens", 1, 0, "CandlestickChart");

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [](QObject *obj, const QUrl &) {
        if (!obj)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.loadFromModule(QStringLiteral("MarketLens"), QStringLiteral("App"));

    return app.exec();
}

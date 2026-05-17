#include "app_controller.hpp"
#include "config.hpp"
#include "graph.hpp"
#include "graph_store.hpp"
#include "ui_context.hpp"
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlExtensionPlugin>


int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    QCoreApplication::setOrganizationName("TT");
    QCoreApplication::setApplicationName("Topic Tracer");

    QIcon appIcon(":/qt/qml/TopicTracer/resources/tt.iconset/icon_128x128.png");
    if (appIcon.isNull()) {
        qDebug() << "failed to load icon";
        return -1;
    }
    app.setWindowIcon(appIcon);
    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);


    engine.setInitialProperties({
        {"major", project_version_major},
        {"minor", project_version_minor},
        {"patch", project_version_patch},
    });


    engine.loadFromModule("TopicTracer", "Main");

    return app.exec();
}

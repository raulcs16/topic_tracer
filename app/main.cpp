#include "app_controller.hpp"
#include "config.hpp"
#include "graph.hpp"
#include "topic_list_model.hpp"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlExtensionPlugin>


int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);


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


    engine.loadFromModule("App", "Main");

    return app.exec();
}

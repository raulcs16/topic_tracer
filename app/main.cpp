#include "topic_graph.hpp"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlExtensionPlugin>
Q_IMPORT_PLUGIN(TopicLibPlugin);


int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);

    TopicGraph topicGraph;


    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("App", "Main");

    return app.exec();
}

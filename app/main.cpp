#include "topic_graph.hpp"
#include "topic_graph_controller.hpp"
#include "topic_list_model.hpp"
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtQml/QQmlExtensionPlugin>


int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);


    TopicGraph myGraph;

    myGraph.addTopic("DesignPatterns");
    myGraph.addTopic("AdapterPattern", Topic_Type::Concrete);


    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);


    // engine.setInitialProperties(
    //     {{"topicListModel", QVariant::fromValue(&topicListModel)},
    //      {"graphController", QVariant::fromValue(&graphController)}});


    engine.loadFromModule("App", "Main");

    return app.exec();
}

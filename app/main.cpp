#include "app_controller.hpp"
#include "command_autocompleter.hpp"
#include "config.hpp"
#include "graph.hpp"
#include "graph_store.hpp"
#include "ui_context.hpp"
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


    GraphStore *store = new GraphStore(&engine);
    UIContext *uiContext = new UIContext(store, &engine);
    AppController *appController = new AppController(uiContext, &engine);

    CommandAutoCompleter *autoCompleter = new CommandAutoCompleter(store, &engine);


    engine.setInitialProperties({
        {"major", project_version_major},
        {"minor", project_version_minor},
        {"patch", project_version_patch},
        {"auto_completer", QVariant::fromValue(autoCompleter)},
        {"controller", QVariant::fromValue(appController)},
    });


    engine.loadFromModule("App", "Main");

    return app.exec();
}

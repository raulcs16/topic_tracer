
#include "cli_service.hpp"
#include "cli_term_controller.hpp"
#include "terminal_controller.hpp"
#include "terminal_list_model.hpp"
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

    auto cli = std::make_unique<CLIService>();


    TerminalListModel terminal(&engine);
    TerminalController *controller =
        new CLITerminalController(std::move(cli), &terminal, &engine);

    engine.setInitialProperties({{"controller", QVariant::fromValue(controller)}});


    engine.loadFromModule("Terminal", "Terminal");

    return app.exec();
}

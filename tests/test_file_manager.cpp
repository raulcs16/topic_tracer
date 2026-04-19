#include <QJsonDocument>
#include <QJsonObject>
#include <QTemporaryDir>
#include <catch2/catch_template_test_macros.hpp>
#include <catch2/catch_test_macros.hpp>

#include "file_manager_dev.hpp"
#include "file_manager_qt.hpp"

// We use TEMPLATE_TEST_CASE to run the same logic for both implementations
TEMPLATE_TEST_CASE("FileManager Implementation Lifecycle",
                   "[FileManager]",
                   FileManagerDev,
                   FileManagerQt) {

    // 1. Setup a sandbox directory so tests don't interfere with real data
    QTemporaryDir tempDir;
    REQUIRE(tempDir.isValid());

    // 2. Initialize the specific implementation
    // Both constructors can take a path/name string
    TestType fileManager(tempDir.path());

    const QString fileName = "test_data.json";
    QJsonObject obj;
    obj["status"] = "ok";
    obj["val"] = 42;
    QByteArray testData = QJsonDocument(obj).toJson();

    SECTION("Base path is valid") {
        QString path = fileManager.getBaseDataPath();
        REQUIRE_FALSE(path.isEmpty());
        REQUIRE(QDir(path).exists());
    }

    SECTION("File saving and existence") {
        REQUIRE(fileManager.saveFile(fileName, testData));
        REQUIRE(fileManager.exists(fileName));

        QStringList files = fileManager.listFiles();
        REQUIRE(files.contains(fileName));
    }

    SECTION("Loading content matches saved data") {
        fileManager.saveFile(fileName, testData);

        QByteArray loadedData = fileManager.loadFile(fileName);
        REQUIRE(loadedData == testData);

        // Verify JSON integrity
        auto doc = QJsonDocument::fromJson(loadedData);
        REQUIRE(doc.object()["val"].toInt() == 42);
    }

    SECTION("File removal works correctly") {
        fileManager.saveFile(fileName, testData);
        REQUIRE(fileManager.exists(fileName));

        REQUIRE(fileManager.removeFile(fileName));
        REQUIRE_FALSE(fileManager.exists(fileName));
        REQUIRE_FALSE(fileManager.listFiles().contains(fileName));
    }

    SECTION("Loading non-existent file returns empty") {
        QByteArray missing = fileManager.loadFile("ghost_file.bin");
        REQUIRE(missing.isEmpty());
    }
}
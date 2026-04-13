#include "file_manager_dev.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <catch2/catch_test_macros.hpp>

FileManagerDev fileManager(DEV_DATA_PATH);
QString testFile = "testFile.json";
bool createTestFile() {
    QJsonObject test;
    test["id"] = 1;
    test["name"] = "test";
    QJsonDocument doc(test);
    return fileManager.saveFile(testFile, doc.toJson());
}
bool deleteTestFile() { return fileManager.removeFile(testFile); }

TEST_CASE("return absolute path") {
    auto path = fileManager.getBaseDataPath().toStdString();
    REQUIRE(path == "/Users/home/Projects/topic_tracer/data");
}
TEST_CASE("return list of files") {
    QStringList list = fileManager.listFiles();
    REQUIRE(list.size() > 0);
}
TEST_CASE("save & delete file to directory") {
    QStringList before = fileManager.listFiles();
    bool save = createTestFile();
    QStringList after = fileManager.listFiles();
    REQUIRE(save);
    REQUIRE(before.length() < after.length());
    bool deleted = deleteTestFile();
    after = fileManager.listFiles();
    REQUIRE(deleted);
    REQUIRE(before.length() == after.length());
}
TEST_CASE("read existing file") {
    createTestFile();
    QByteArray array = fileManager.loadFile(testFile);
    REQUIRE(!array.isEmpty());
    deleteTestFile();
}
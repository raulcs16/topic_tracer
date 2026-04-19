#include "file_manager_qt.hpp"
#include <QDir>
#include <QFile>
#include <QStandardPaths>

FileManagerQt::FileManagerQt(const QString &appName) {
    QString base = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    if (!base.endsWith(appName)) {
        m_basePath = base + "/" + appName;
    } else {
        m_basePath = base;
    }

    QDir dir(m_basePath);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

QString FileManagerQt::getBaseDataPath() const { return m_basePath; }

QStringList FileManagerQt::listFiles() const {
    QDir dir(m_basePath);
    return dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
}

bool FileManagerQt::saveFile(const QString &fileName, const QByteArray &data) {
    QDir dir(m_basePath);
    QString finalPath = dir.filePath(fileName);
    QString tempPath = dir.filePath("temp." + fileName);

    QFile tempFile(tempPath);
    if (!tempFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    tempFile.write(data);
    tempFile.close();

    // Safe replace logic
    if (QFile::exists(finalPath)) {
        if (!QFile::remove(finalPath)) {
            return false;
        }
    }
    return tempFile.rename(finalPath);
}

QByteArray FileManagerQt::loadFile(const QString &fileName) {
    QFile file(QDir(m_basePath).filePath(fileName));
    if (!file.open(QIODevice::ReadOnly)) {
        return QByteArray();
    }
    return file.readAll();
}

bool FileManagerQt::exists(const QString &fileName) const {
    return QFile::exists(QDir(m_basePath).filePath(fileName));
}

bool FileManagerQt::removeFile(const QString &fileName) {
    QString finalPath = QDir(m_basePath).filePath(fileName);
    if (QFile::exists(finalPath)) {
        return QFile::remove(finalPath);
    }
    return true;
}
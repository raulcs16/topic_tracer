#include "file_manager_dev.hpp"
#include <QDir>
#include <QFile>


FileManagerDev::FileManagerDev(const QString &relativePath) : m_path(relativePath) {
    QDir dir(m_path);
    if (!dir.exists()) {
        dir.mkpath(".");
    }
}

QString FileManagerDev::getBaseDataPath() const { return QDir(m_path).absolutePath(); }
QStringList FileManagerDev::listFiles() const {
    QDir dir(m_path);
    return dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
}
bool FileManagerDev::saveFile(const QString &fileName, const QByteArray &data) {
    QString finalPath = m_path + "/" + fileName;
    QString tempPath = m_path + "/temp." + fileName;

    QFile tempFile(tempPath);
    if (!tempFile.open(QIODevice::WriteOnly)) {
        return false;
    }
    tempFile.write(data);
    tempFile.close();

    if (QFile::exists(finalPath)) {
        if (!QFile::remove(finalPath)) {
            return false;
        }
    }
    return tempFile.rename(finalPath);
}
QByteArray FileManagerDev::loadFile(const QString &fileName) {
    QString finalPath = m_path + "/" + fileName;
    QFile file(finalPath);
    if (!file.open(QIODevice::ReadOnly)) {
        return QByteArray(nullptr);
    }
    QByteArray array = file.readAll();
    file.close();
    return array;
}
bool FileManagerDev::exists(const QString &fileName) const {
    QString finalPath = m_path + "/" + fileName;
    return QFile::exists(finalPath);
}
bool FileManagerDev::removeFile(const QString &fileName) {
    QString finalPath = m_path + "/" + fileName;
    if (QFile::exists(finalPath)) {
        return QFile::remove(finalPath);
    }
    return true;
}
#include "file_manager_qt.hpp"
#include <QDir>
#include <QFile>


FileManagerQt::FileManagerQt(const QString &appName) : m_appName(appName) {
    QDir().mkpath(getBaseDataPath());
}

QString FileManagerQt::getBaseDataPath() const override {
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/" +
           m_appName;
}
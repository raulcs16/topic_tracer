#pragma once
#include "ifile_manager.hpp"

class FileManagerQt : public IFileManager {
public:
    FileManagerQt(const QString &appName);
    QString getBaseDataPath() const override;
    virtual QStringList listFiles() const = 0;

    virtual bool saveFile(const QString &fileName, const QByteArray &data) = 0;
    virtual QByteArray loadFile(const QString &fileName) = 0;
    virtual bool exists(const QString &fileName) const = 0;
    virtual bool removeFile(const QString &fileName) = 0;

private:
    QString m_appName;
};
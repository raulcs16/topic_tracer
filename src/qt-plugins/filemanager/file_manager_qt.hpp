#pragma once
#include "ifile_manager.hpp"

class FileManagerQt : public IFileManager {
public:
    FileManagerQt(const QString &appName);
    QString getBaseDataPath() const override;
    QStringList listFiles() const override;
    bool saveFile(const QString &fileName, const QByteArray &data) override;
    QByteArray loadFile(const QString &fileName) override;
    bool exists(const QString &fileName) const override;
    bool removeFile(const QString &fileName) override;

private:
    QString m_basePath;
};
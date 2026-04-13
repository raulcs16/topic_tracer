#pragma once
#include "ifile_manager.hpp"

class FileManagerDev : public IFileManager {
public:
    FileManagerDev(const QString &relativePath);
    QString getBaseDataPath() const override;
    QStringList listFiles() const override;

    bool saveFile(const QString &fileName, const QByteArray &data) override;
    QByteArray loadFile(const QString &fileName) override;
    bool exists(const QString &fileName) const override;
    bool removeFile(const QString &fileName) override;

private:
    QString m_path;
};
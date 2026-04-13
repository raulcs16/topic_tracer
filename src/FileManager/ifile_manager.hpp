#pragma once
#include <QString>
#include <QStringList>
class IFileManager {
public:
    virtual ~IFileManager() = default;

    // Path Logic
    virtual QString getBaseDataPath() const = 0;
    virtual QStringList listFiles() const = 0;

    // Disk Operations
    virtual bool saveFile(const QString &fileName, const QByteArray &data) = 0;
    virtual QByteArray loadFile(const QString &fileName) = 0;
    virtual bool exists(const QString &fileName) const = 0;
    virtual bool removeFile(const QString &fileName) = 0;
};
#pragma once

#include <QString>
#include <QFile>
#include <QByteArray>

class FileUtil
{
public:
    FileUtil() = default;
    ~FileUtil() = default;

    static QByteArray readBinaryFile(const QString& filePath);
};

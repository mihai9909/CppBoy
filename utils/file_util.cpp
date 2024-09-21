#include "file_util.h"

QByteArray FileUtil::readBinaryFile(const QString& filePath)
{
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly)) {
        return nullptr;
    }

    QByteArray content = file.readAll();
    file.close();

    return content;
}

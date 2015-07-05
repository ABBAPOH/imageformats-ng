#include "writeoptions.h"

WriteOptions::WriteOptions()
{

}

bool WriteOptions::isEmpty() const
{
    return _options.isEmpty();
}

QString WriteOptions::mimeType() const
{
    return _options.value(MimeType).toString();
}

void WriteOptions::setMimeType(const QString &mimeType)
{
    _options.insert(MimeType, mimeType);
}

QByteArray WriteOptions::subType() const
{
    return _options.value(SubType).toByteArray();
}

void WriteOptions::setSubType(const QByteArray &type)
{
    _options.insert(SubType, type);
}


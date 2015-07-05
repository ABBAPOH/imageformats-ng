#include "writeoptions.h"
#include "imagedocument_p.h"

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

QVector<QByteArray> WriteOptions::supportedSubTypes() const
{
    if (mimeType().isEmpty())
        return QVector<QByteArray>();

    auto plugin = ImageIOHandlerDatabase::instance()->plugin(mimeType());
    if (!plugin)
        return QVector<QByteArray>();
    return plugin->subTypes();
}

int WriteOptions::gamma() const
{
    return _options.value(Gamma).toInt();
}

void WriteOptions::setGamma(int gamma)
{
    _options.insert(Gamma, gamma);
}

int WriteOptions::quality() const
{
    return _options.value(Quality).toInt();
}

void WriteOptions::setQuality(int quality)
{
    _options.insert(Quality, quality);
}

QStringList WriteOptions::supportedMimeTypes()
{
    return ImageIOHandlerDatabase::instance()->availableMimeTypes(ImageIOHandlerPlugin::CanWrite);
}

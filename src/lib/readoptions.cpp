#include "readoptions.h"
#include "imagedocument_p.h"

ReadOptions::ReadOptions()
{

}

ReadOptions::ReadOptions(const QString &mimeType)
{
    setMimeType(mimeType);
}

QColor ReadOptions::backgroundColor() const
{
    return _options.value(BackgroundColor).value<QColor>();
}

void ReadOptions::setBackgroundColor(const QColor &color)
{
    _options.insert(BackgroundColor, color);
}

QStringList ReadOptions::supportedMimeTypes()
{
    return ImageIOHandlerDatabase::instance()->availableMimeTypes(ImageIOHandlerPlugin::CanRead);
}


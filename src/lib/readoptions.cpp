#include "readoptions.h"
#include "imagedocument_p.h"

ReadOptions::ReadOptions()
{

}

ReadOptions::ReadOptions(const QString &mimeType)
{
    setMimeType(mimeType);
}

bool ReadOptions::supportsOption(ReadOptions::Option option) const
{
    if (mimeType().isEmpty())
        return false;

    auto plugin = ImageIOHandlerDatabase::instance()->plugin(mimeType());
    if (!plugin)
        return false;
    return plugin->supportsOption(option);
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


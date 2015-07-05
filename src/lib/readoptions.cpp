#include "readoptions.h"

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


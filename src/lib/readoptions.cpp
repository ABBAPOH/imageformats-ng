#include "readoptions.h"
#include "imagedocument_p.h"

ReadOptions::ReadOptions()
{

}

QColor ReadOptions::backgroundColor() const
{
    return _options.value(BackgroundColor).value<QColor>();
}

void ReadOptions::setBackgroundColor(const QColor &color)
{
    _options.insert(BackgroundColor, color);
}


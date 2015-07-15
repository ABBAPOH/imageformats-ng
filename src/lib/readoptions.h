#ifndef READOPTIONS_H
#define READOPTIONS_H

#include <QtCore/QHash>
#include <QtCore/QRect>
#include <QtCore/QVariant>
#include <QtGui/QColor>

class ReadOptions
{
public:
    enum Option {
        Quality,
        BackgroundColor
    };

    ReadOptions();

    bool isEmpty() const { return _options.isEmpty(); }

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

private:
    QHash<Option, QVariant> _options;
};

#endif // READOPTIONS_H

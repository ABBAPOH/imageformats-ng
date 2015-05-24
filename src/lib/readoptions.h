#ifndef READOPTIONS_H
#define READOPTIONS_H

#include <QtCore/QHash>
#include <QtCore/QRect>
#include <QtCore/QVariant>

class ReadOptions
{
public:
    enum Option {
        ClipRect,
    };

    ReadOptions();

    bool isEmpty() const { return _options.isEmpty(); }

    QRect clipRect() const { return _options.value(ClipRect).toRect(); }
    void setRect(const QRect &rect) { _options.insert(ClipRect, rect); }

private:
    QHash<Option, QVariant> _options;
};

#endif // READOPTIONS_H

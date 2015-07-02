#ifndef READOPTIONS_H
#define READOPTIONS_H

#include <QtCore/QHash>
#include <QtCore/QRect>
#include <QtCore/QVariant>

class ReadOptions
{
public:
    enum Option {
        MimeType,
        ClipRect,
    };

    ReadOptions();
    ReadOptions(const QString &mimeType);

    bool isEmpty() const { return _options.isEmpty(); }

    QString mimeType() const { return _options.value(MimeType).toString(); }
    void setMimeType(const QString &mimeType) { _options.insert(MimeType, mimeType); }

    QRect clipRect() const { return _options.value(ClipRect).toRect(); }
    void setRect(const QRect &rect) { _options.insert(ClipRect, rect); }

private:
    QHash<Option, QVariant> _options;
};

#endif // READOPTIONS_H

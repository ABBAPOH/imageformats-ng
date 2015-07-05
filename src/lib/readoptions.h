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
        MimeType,
        Quality,
        BackgroundColor
    };

    ReadOptions();
    ReadOptions(const QString &mimeType);

    bool isEmpty() const { return _options.isEmpty(); }

    QString mimeType() const { return _options.value(MimeType).toString(); }
    void setMimeType(const QString &mimeType) { _options.insert(MimeType, mimeType); }

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &color);

private:
    QHash<Option, QVariant> _options;
};

#endif // READOPTIONS_H

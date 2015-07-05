#ifndef WRITEOPTIONS_H
#define WRITEOPTIONS_H

#include <QtCore/QHash>
#include <QtCore/QVariant>

class WriteOptions
{
public:
    enum Option {
        MimeType,
        CompressionRatio,
        Gamma,
        Quality,
        SubType
    };

    WriteOptions();

    bool isEmpty() const;

    QString mimeType() const;
    void setMimeType(const QString &mimeType);

    QByteArray subType() const;
    void setSubType(const QByteArray &type);

//    int gamma() const;
//    void setGamma(int gamma);

private:
    QHash<Option, QVariant> _options;
};

#endif // WRITEOPTIONS_H

#ifndef WRITEOPTIONS_H
#define WRITEOPTIONS_H

#include <QtCore/QHash>
#include <QtCore/QVariant>

class LIB_EXPORT WriteOptions
{
public:
    enum Option {
        CompressionRatio,
        Gamma,
        Quality,
        SubType
    };

    WriteOptions();

    bool isEmpty() const;

    int compression() const;
    void setCompression(int compression);

    int gamma() const;
    void setGamma(int gamma);

    int quality() const;
    void setQuality(int quality);

private:
    QHash<Option, QVariant> _options;
};

#endif // WRITEOPTIONS_H

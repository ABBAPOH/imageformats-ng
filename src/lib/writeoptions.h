#ifndef WRITEOPTIONS_H
#define WRITEOPTIONS_H

#include <QtCore/QHash>
#include <QtCore/QVariant>

class WriteOptions
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

    int gamma() const;
    void setGamma(int gamma);

    int quality() const;
    void setQuality(int quality);

private:
    QHash<Option, QVariant> _options;
};

#endif // WRITEOPTIONS_H

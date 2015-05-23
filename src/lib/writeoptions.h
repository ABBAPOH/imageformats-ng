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

    bool isEmpty() const { return _options.isEmpty(); }

    QByteArray subType() const { return _options.value(SubType).toByteArray(); }
    void setSubType(const QByteArray &type) { _options.insert(SubType, type); }

//    int gamma() const;
//    void setGamma(int gamma);

private:
    QHash<Option, QVariant> _options;
};

#endif // WRITEOPTIONS_H
#include "writeoptions.h"
#include "imagedocument_p.h"

WriteOptions::WriteOptions()
{

}

bool WriteOptions::isEmpty() const
{
    return _options.isEmpty();
}

int WriteOptions::compression() const
{
    return _options.value(CompressionRatio, -1).toInt();
}

void WriteOptions::setCompression(int compression)
{
    _options.insert(CompressionRatio, compression);
}

int WriteOptions::gamma() const
{
    return _options.value(Gamma, -1).toInt();
}

void WriteOptions::setGamma(int gamma)
{
    _options.insert(Gamma, gamma);
}

int WriteOptions::quality() const
{
    return _options.value(Quality, -1).toInt();
}

void WriteOptions::setQuality(int quality)
{
    _options.insert(Quality, quality);
}

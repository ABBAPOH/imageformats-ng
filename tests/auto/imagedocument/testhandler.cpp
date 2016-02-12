#include "testhandler.h"
#include "testhandler_p.h"

#include <QtPlugin>

QDataStream &operator <<(QDataStream &stream, const TestImageData &data)
{
    stream << data.magic;
    stream << data.subType;
    stream << quint32(data.type);
    stream << data.size;
    stream << data.name;
    stream << quint32(data.imageFormat);
    stream << data.imageCount;
    stream << data.mipmapCount;
    stream << data.loopCount;
    stream << data.images;

    stream << data.compression;
    stream << data.gamma;
    stream << data.quality;

    return stream;
}

QDataStream &operator >>(QDataStream &stream, TestImageData &data)
{
    quint32 magic = 0;
    quint32 value;
    stream >> magic;
    if (magic != 0)
        return stream;

    data = TestImageData();

    data.magic = magic;
    stream >> data.subType;
    stream >> value;
    data.type = ImageContents::Type(value);
    stream >> data.size;
    stream >> data.name;
    stream >> value;
    data.imageFormat = QImage::Format(value);
    stream >> data.imageCount;
    stream >> data.mipmapCount;
    stream >> data.loopCount;
    stream >> data.images;

    stream >> data.compression;
    stream >> data.gamma;
    stream >> data.quality;

    return stream;
}

bool TestHandler::canRead()
{
    return true;
}

bool TestHandler::readHeader(ImageContents &contents)
{
    QDataStream s(device());
    s >> _data;
    if (s.status() != QDataStream::Ok)
        return false;

    setSubType(_data.subType);

    contents.setType(_data.type);
    contents.setSize(_data.size);
    contents.setName(_data.name);
    contents.setImageFormat(_data.imageFormat);
    contents.setImageCount(_data.imageCount);
    contents.setMipmapCount(_data.mipmapCount);
    contents.setLoopCount(_data.loopCount);

    return true;
}

bool TestHandler::read(ImageContents &contents, const ImageOptions &options)
{
    Q_UNUSED(contents);
    Q_UNUSED(options);

    for (int level = 0, i = 0; level < _data.mipmapCount; ++level) {
        for (int index = 0; index < _data.imageCount; ++index, i++) {
            contents.setImage(_data.images[i], index, level);
        }
    }

    return true;
}

bool TestHandler::write(const ImageContents &contents, const ImageOptions &options)
{
    Q_UNUSED(contents);
    Q_UNUSED(options);

    _data = TestImageData();

    _data.type = contents.type();
    _data.size = contents.size();
    _data.name = contents.name();
    _data.imageFormat = contents.imageFormat();
    _data.imageCount = contents.imageCount();
    _data.mipmapCount = contents.mipmapCount();
    _data.loopCount = contents.loopCount();

    _data.images.resize(_data.mipmapCount * _data.imageCount);

    for (int level = 0, i = 0; level < _data.mipmapCount; ++level) {
        for (int index = 0; index < _data.imageCount; ++index, i++) {
            _data.images[i] = contents.image(index, level);
        }
    }

    _data.compression = options.compression();
    _data.gamma = options.gamma();
    _data.quality = options.quality();

    QDataStream stream(device());
    stream << _data;

    return true;
}

TestHandlerPlugin::TestHandlerPlugin()
{
}

ImageIOHandler *TestHandlerPlugin::create(QIODevice *device, const QMimeType &mimeType)
{
    Q_UNUSED(device);
    Q_UNUSED(mimeType);
    return new TestHandler();
}

ImageIOHandlerPlugin::Capabilities TestHandlerPlugin::capabilities(QIODevice *device,
                                                                   const QMimeType &mimeType) const
{
    Q_UNUSED(device);
    Q_UNUSED(mimeType);
    return ImageIOHandlerPlugin::Capabilities(ImageIOHandlerPlugin::CanRead
                                              | ImageIOHandlerPlugin::CanWrite);
}

Q_IMPORT_PLUGIN(TestHandlerPlugin)

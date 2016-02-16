#ifndef IMAGEIOHANDLERDATABASE_H
#define IMAGEIOHANDLERDATABASE_H

#include "imageio_global.h"

#include <ImageIOHandler>

class IMAGEIO_EXPORT ImageIOHandlerDatabase
{
public:
    ImageIOHandlerDatabase();
    ~ImageIOHandlerDatabase();

    std::unique_ptr<ImageIOHandler> create(QIODevice *device, const QMimeType &mimeType);
    QVector<QMimeType> availableMimeTypes(ImageIOHandlerPlugin::Capabilities caps) const;
    ImageIOHandlerPlugin *plugin(const QString &mimeType) const;
    void registerPlugin(const QString &mimeType, ImageIOHandlerPlugin *plugin);

    QVector<ImageFormatInfo> supportedImageFormats() const;
    Optional<ImageFormatInfo> imageFormat(const QMimeType &mt) const;
    static ImageFormatInfo getInfo(const QMimeType &mt, ImageIOHandlerPlugin *plugin);

    static ImageIOHandlerDatabase *instance();

private:

    QHash<QString, ImageIOHandlerPlugin *> map;
};

#endif // IMAGEIOHANDLERDATABASE_H
#ifndef IMAGEIOHANDLERDATABASE_H
#define IMAGEIOHANDLERDATABASE_H

#include <ImageIOHandler>

class IMAGEDOCUMENT_EXPORT ImageIOHandlerDatabase
{
public:
    ImageIOHandlerDatabase();
    ~ImageIOHandlerDatabase();

    std::unique_ptr<ImageIOHandler> create(QIODevice *device, const QMimeType &mimeType);
    QVector<QMimeType> availableMimeTypes(ImageIOHandlerPlugin::Capabilities caps) const;
    ImageIOHandlerPlugin *plugin(const QString &mimeType) const;
    void registerPlugin(const QString &mimeType, ImageIOHandlerPlugin *plugin);

    static ImageIOHandlerDatabase *instance();

private:

    QHash<QString, ImageIOHandlerPlugin *> map;
};

#endif // IMAGEIOHANDLERDATABASE_H

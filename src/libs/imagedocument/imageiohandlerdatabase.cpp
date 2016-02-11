#include "imageiohandlerdatabase.h"

#include "defaulthandler.h"

#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QJsonArray>
#include <QtCore/QMimeDatabase>
#include <QtCore/QPluginLoader>

ImageIOHandlerDatabase::ImageIOHandlerDatabase()
{
    for (auto staticPlugin : QPluginLoader::staticPlugins()) {
        auto plugin = staticPlugin.instance();
        auto handlerPlugin = qobject_cast<ImageIOHandlerPlugin *>(plugin);
        if (!handlerPlugin)
            continue;

        const auto metaData = staticPlugin.metaData().value("MetaData").toObject();
        const auto mimeTypes = metaData.value("MimeTypes").toArray();
        for (auto mimeType : mimeTypes) {
            registerPlugin(mimeType.toString(), handlerPlugin);
        }
    }

    for (const QString &folder : qApp->libraryPaths()) {
        QDir dir(folder);
        if (!dir.cd("imageformats2"))
            continue;
        for (const QString &fileName : dir.entryList(QDir::Files)) {
            QPluginLoader loader(dir.absoluteFilePath(fileName));
            const auto metaData = loader.metaData().value("MetaData").toObject();
            const auto mimeTypes = metaData.value("MimeTypes").toArray();
            if (mimeTypes.isEmpty()) {
                qWarning() << "File" << dir.absoluteFilePath(fileName)
                           << "does not contain 'MimeTypes' key";
                continue;
            }
            QObject *plugin = loader.instance();
            if (!plugin) {
                qWarning() << "File" << dir.absoluteFilePath(fileName) << "is not a Qt plugin";
                continue;
            }

            auto handlerPlugin = qobject_cast<ImageIOHandlerPlugin *>(plugin);
            if (!handlerPlugin) {
                qWarning() << "File" << dir.absoluteFilePath(fileName)
                           << "does not contain an imageformat plugin";
                continue;
            }
            for (auto mimeTypeName : mimeTypes) {
                registerPlugin(mimeTypeName.toString(), handlerPlugin);
            }
        }
    }

    map.insert("image/gif", new DefaultHandlerPlugin());
}

ImageIOHandlerDatabase::~ImageIOHandlerDatabase()
{
    qDeleteAll(map);
}

std::unique_ptr<ImageIOHandler> ImageIOHandlerDatabase::create(QIODevice *device, const QMimeType &mimeType)
{
    auto plugin = map.value(mimeType.name());
    if (!plugin)
        return std::unique_ptr<ImageIOHandler>();
    return std::unique_ptr<ImageIOHandler>(plugin->create(device, mimeType));
}

QVector<QMimeType> ImageIOHandlerDatabase::availableMimeTypes(ImageIOHandlerPlugin::Capabilities caps) const
{
    QVector<QMimeType> result;
    for (auto it = map.begin(), end = map.end(); it != end; it++) {
        auto mt = QMimeDatabase().mimeTypeForName(it.key());
        if (it.value()->capabilities(nullptr, mt) & caps)
            result.append(mt);
    }
    return result;
}

ImageIOHandlerPlugin *ImageIOHandlerDatabase::plugin(const QString &mimeType) const
{
    return map.value(mimeType);
}

void ImageIOHandlerDatabase::registerPlugin(const QString &mimeType, ImageIOHandlerPlugin *plugin)
{
    Q_ASSERT(plugin);

    const QMimeType type = QMimeDatabase().mimeTypeForName(mimeType);
    if (!type.isValid()) {
        qWarning() << "ImageIOHandlerDatabase::registerPlugin: Invalid mimetype" << mimeType;
        return;
    }

    map.insert(mimeType, plugin);
}

Q_GLOBAL_STATIC(ImageIOHandlerDatabase, static_instance)
ImageIOHandlerDatabase *ImageIOHandlerDatabase::instance()
{
    return static_instance();
}

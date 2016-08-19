#include "imagedocument.h"
#include "imagedocument_p.h"

#include <QtConcurrent/QtConcurrent>

ImageDocumentPrivate::ImageDocumentPrivate(ImageDocument *qq) :
    AbstractDocumentPrivate(qq)
{
}

ImageDocument::ImageDocument(QObject *parent) :
    AbstractDocument(*new ImageDocumentPrivate(this), parent)
{
}

ImageDocument::~ImageDocument()
{
}

ImageContents ImageDocument::contents() const
{
    Q_D(const ImageDocument);
    return d->contents;
}

void ImageDocument::setContents(const ImageContents &contents)
{
    Q_D(ImageDocument);
    if (d->contents == contents)
        return;

    d->contents = contents;
    emit contentsChanged();
}

void ImageDocument::doOpen(const QUrl &url, const QVariantMap &options)
{
    if (!url.isLocalFile()) {
        finishOpen(false);
        return;
    }

    using Result = Optional<ImageContents>;

    auto worker = [](const QString &path)
    {
        ImageIO io(path);
        return io.read();
    };

    auto finisher = [this]()
    {
        const auto watcher = static_cast<QFutureWatcher<Result> *>(sender());
        const auto result = watcher->future().result();
        if (result)
            setContents(*result);
        watcher->deleteLater();
        finishOpen(result);
    };

    auto watcher = new QFutureWatcher<Result>(this);
    connect(watcher, &QFutureWatcherBase::finished, this, finisher);
    watcher->setFuture(QtConcurrent::run(worker, url.toLocalFile()));
}

void ImageDocument::doSave(const QUrl &url, const QVariantMap &options)
{
    if (!url.isLocalFile()) {
        finishSave(false);
        return;
    }

    const auto subType = options["subType"].toByteArray();
    const auto imageOptions = options["imageOptions"].value<ImageOptions>();

    using Result = bool;

    auto worker = [subType, imageOptions](const QString &path, const ImageContents &contents)
    {
        ImageIO io(path);
        io.setSubType(subType);
        return io.write(contents, imageOptions);
    };

    auto finisher = [this]()
    {
        const auto watcher = static_cast<QFutureWatcher<Result> *>(sender());
        const auto result = watcher->future().result();
        watcher->deleteLater();
        finishSave(result);
    };

    auto watcher = new QFutureWatcher<Result>(this);
    connect(watcher, &QFutureWatcherBase::finished, this, finisher);
    watcher->setFuture(QtConcurrent::run(worker, url.toLocalFile(), contents()));
}

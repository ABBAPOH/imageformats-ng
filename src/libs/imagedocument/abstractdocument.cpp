#include "abstractdocument.h"
#include "abstractdocument_p.h"

#include <QtCore/QMimeDatabase>
#include <memory>

void AbstractDocumentPrivate::setOpened(bool opened)
{
    Q_Q(AbstractDocument);

    if (this->opened == opened)
        return;
    this->opened = opened;
    emit q->openedChanged(opened);
}

AbstractDocument::AbstractDocument(QObject *parent) :
    QObject(parent),
    d_ptr(new AbstractDocumentPrivate(this))
{
}

AbstractDocument::AbstractDocument(AbstractDocumentPrivate &dd, QObject *parent) :
    QObject(parent),
    d_ptr(&dd)
{
}

void AbstractDocument::finishOpen(bool ok)
{
    Q_D(AbstractDocument);

    d->setOpened(ok);
    emit openFinished(ok);
}

void AbstractDocument::finishSave(bool ok)
{
    emit saveFinished(ok);
}

AbstractDocument::~AbstractDocument()
{
}

QUrl AbstractDocument::url() const
{
    Q_D(const AbstractDocument);
    return d->url;
}

void AbstractDocument::setUrl(const QUrl &url)
{
    Q_D(AbstractDocument);
    if (d->url == url)
        return;

    d->url = url;
    const auto oldMt = d->mimeType;
    d->mimeType = QMimeDatabase().mimeTypeForUrl(url);

    emit urlChanged(d->url);
    if (oldMt != d->mimeType)
        emit mimeTypeChanged(d->mimeType);
}

QMimeType AbstractDocument::mimeType() const
{
    Q_D(const AbstractDocument);
    return d->mimeType;
}

void AbstractDocument::setMimeType(const QMimeType &mimeType)
{
    Q_D(AbstractDocument);
    if (d->mimeType == mimeType)
        return;
    d->mimeType = mimeType;
    emit mimeTypeChanged(d->mimeType);
}

void AbstractDocument::setMimeType(const QString &name)
{
    setMimeType(QMimeDatabase().mimeTypeForName(name));
}

bool AbstractDocument::isOpened() const
{
    Q_D(const AbstractDocument);
    return d->opened;
}

bool AbstractDocument::isModified() const
{
    Q_D(const AbstractDocument);
    return d->modified;
}

void AbstractDocument::open()
{
    Q_D(AbstractDocument);
    if (d->url.isEmpty()) {
        finishOpen(false);
    }

    doOpen();
}

void AbstractDocument::save()
{
    Q_D(AbstractDocument);
    if (d->url.isEmpty()) {
        finishSave(false);
    }

    doSave();
}

void AbstractDocument::setModified(bool modified)
{
    Q_D(AbstractDocument);
    if (d->modified == modified)
        return;

    d->modified = modified;
    emit modificationChanged(modified);
}

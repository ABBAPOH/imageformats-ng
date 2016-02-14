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

/*!
    Constructs an AbstractDocument with the given \a parent.
*/
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

/*!
    Destroys the AbstractDocument object.
*/
AbstractDocument::~AbstractDocument()
{
}

/*!
    \fn void AbstractDocument::doOpen(const QUrl &url);

    Reimplement this function to open document contents located at \a url.
*/

/*!
    Reimplement this function to save document contents to the \a url.

    \note An \a url parameter can differ from value returned by url() function (for example, in
    case of autosaving)
*/
void AbstractDocument::doSave(const QUrl &url)
{
    Q_UNUSED(url);
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

    doOpen(d->url);
}

void AbstractDocument::save()
{
    Q_D(AbstractDocument);
    if (d->url.isEmpty()) {
        finishSave(false);
    }

    doSave(d->url);
}

void AbstractDocument::setModified(bool modified)
{
    Q_D(AbstractDocument);
    if (d->modified == modified)
        return;

    d->modified = modified;
    emit modificationChanged(modified);
}

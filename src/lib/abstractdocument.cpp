#include "abstractdocument.h"
#include "abstractdocument_p.h"

#include <QtCore/QMimeDatabase>

AbstractDocumentPrivate::AbstractDocumentPrivate(AbstractDocument *qq) :
    q_ptr(qq)
{
    modified = false;
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

AbstractDocument::~AbstractDocument()
{
}

QIODevice *AbstractDocument::device() const
{
    Q_D(const AbstractDocument);
    return d->device;
}

void AbstractDocument::setDevice(QIODevice *device)
{
    Q_D(AbstractDocument);
    if (d->device == device)
        return;
    d->file.reset();
    d->device = device;
    d->changed();
    emit deviceChanged();
}

QString AbstractDocument::fileName() const
{
    Q_D(const AbstractDocument);
    return d->fileName;
}

void AbstractDocument::setFileName(const QString &fileName)
{
    Q_D(AbstractDocument);
    if (d->fileName == fileName)
        return;

    d->file.reset(new QFile(fileName));
    d->device = d->file.data();
    d->mimeType = QMimeDatabase().mimeTypeForFile(fileName);
    d->changed();
    emit fileNameChanged(fileName);
    emit deviceChanged();
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
    d->changed();
    emit mimeTypeChanged(d->mimeType);
}

void AbstractDocument::setMimeType(const QString &name)
{
    setMimeType(QMimeDatabase().mimeTypeForName(name));
}

bool AbstractDocument::modified() const
{
    Q_D(const AbstractDocument);
    return d->modified;
}

void AbstractDocument::setModified(bool modified)
{
    Q_D(AbstractDocument);
    if (d->modified == modified)
        return;

    d->modified = modified;
    emit modificationChanged(modified);
}

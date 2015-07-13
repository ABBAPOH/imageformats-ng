#include "abstractdocument.h"
#include "abstractdocument_p.h"

#include <QtCore/QMimeDatabase>

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
//    d->killHandler();
    d->device = device;
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
//    d->killHandler();
    d->device = d->file.data();
    d->mimeType = QMimeDatabase().mimeTypeForFile(fileName).name();
    d->fileNameChanged(d->mimeType);
    d->deviceChanged(d->device);
    d->mimeTypeChanged(d->mimeType);
    emit fileNameChanged(fileName);
    emit deviceChanged();
    emit mimeTypeChanged(d->mimeType);
}

QString AbstractDocument::mimeType() const
{
    Q_D(const AbstractDocument);
    return d->mimeType;
}

void AbstractDocument::setMimeType(const QMimeType &mimeType)
{
    Q_D(AbstractDocument);
    if (d->mimeType == mimeType.name())
        return;
//    d->killHandler();
    d->mimeType = mimeType.name();
    d->mimeTypeChanged(d->mimeType);
    emit mimeTypeChanged(d->mimeType);
}

void AbstractDocument::setMimeType(const QString &name)
{
    auto type = QMimeDatabase().mimeTypeForName(name);
    setMimeType(type);
}

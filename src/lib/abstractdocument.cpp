#include "abstractdocument.h"
#include "abstractdocument_p.h"

#include <QtCore/QMimeDatabase>

AbstractDocumentPrivate::AbstractDocumentPrivate(AbstractDocument *qq) :
    q_ptr(qq)
{
    modified = false;
}

QString AbstractDocumentPrivate::errorString(AbstractDocument::Result::ErrorCode code)
{
    switch (code) {
    case AbstractDocument::Result::NoError:
        return AbstractDocument::tr("No error");
    case AbstractDocument::Result::InvalidMimeTypeError:
        return AbstractDocument::tr("Invalid mimetype");
    case AbstractDocument::Result::FileNotFoundError:
        return AbstractDocument::tr("File not found");
    case AbstractDocument::Result::DeviceError:
        return AbstractDocument::tr("Device error");
    case AbstractDocument::Result::UnsupportedMimeTypeError:
        return AbstractDocument::tr("Unsupported format");
    case AbstractDocument::Result::IOError:
        return AbstractDocument::tr("Handler error");
    }
    return QString();
}

bool AbstractDocumentPrivate::ensureDeviceOpened(QIODevice::OpenMode mode)
{
    if ((device->openMode() & mode) != mode) {
        device->close();
        if (!device->open(mode))
            return false;
    }
    return true;
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

AbstractDocument::Result AbstractDocument::open()
{
    Q_D(AbstractDocument);

    if (!d->mimeType.isValid())
        return Result::InvalidMimeTypeError;

    if (!d->device)
        return Result::DeviceError;

    if (d->file && !d->file->exists())
        return Result::FileNotFoundError;

    if (!supportedInputMimetypes().contains(d->mimeType))
        return Result::UnsupportedMimeTypeError;

    if (!d->ensureDeviceOpened(QIODevice::ReadOnly))
        return Result::DeviceError;

    if (!read())
        return Result::IOError;

    return Result();
}

AbstractDocument::Result AbstractDocument::save()
{
    Q_D(AbstractDocument);

    if (!d->mimeType.isValid())
        return Result::InvalidMimeTypeError;

    if (!d->device)
        return Result::DeviceError;

    if (!supportedOutputMimetypes().contains(d->mimeType))
        return Result::UnsupportedMimeTypeError;

    if (!d->ensureDeviceOpened(QIODevice::WriteOnly))
        return Result::DeviceError;

    if (!write())
        return Result::IOError;

    return Result();
}

void AbstractDocument::setModified(bool modified)
{
    Q_D(AbstractDocument);
    if (d->modified == modified)
        return;

    d->modified = modified;
    emit modificationChanged(modified);
}

QString AbstractDocument::Result::errorString() const
{
    return AbstractDocumentPrivate::errorString(_error);
}

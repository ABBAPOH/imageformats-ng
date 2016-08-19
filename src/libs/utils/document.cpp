#include "document.h"
#include "document_p.h"

#include <memory>

/*!
    Constructs an AbstractDocument with the given \a parent.
*/
Document::Document(QObject *parent) :
    QObject(parent),
    d_ptr(new DocumentPrivate(this))
{
}

Document::Document(DocumentPrivate &dd, QObject *parent) :
    QObject(parent),
    d_ptr(&dd)
{
}

/*!
    Destroys the AbstractDocument object.
*/
Document::~Document()
{
}

bool Document::isModified() const
{
    Q_D(const Document);
    return d->modified;
}

void Document::setModified(bool modified)
{
    Q_D(Document);
    if (d->modified == modified)
        return;

    d->modified = modified;
    emit modificationChanged(modified);
}

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

/*!
    Destroys the AbstractDocument object.
*/
Document::~Document()
{
}

/*!
    \property Document::isModified
    This property holds whether the document has been modified by the user.
    By default, this property is false.
    \sa modificationChanged()
*/

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

/*!
    \fn Document::modificationChanged(bool changed)

    This signal is emitted whenever the content of the document
    changes in a way that affects the modification state. If \a
    changed is true, the document has been modified; otherwise it is
    false.

    For example, calling setModified(false) on a document and then
    changing a document causes the signal to get emitted. If you undo that
    operation, causing the document to return to its original
    unmodified state, the signal will get emitted again.
*/

/*!
    Constructs an AbstractDocument with the given \a parent and private class \a dd.
*/
Document::Document(DocumentPrivate &dd, QObject *parent) :
    QObject(parent),
    d_ptr(&dd)
{
}

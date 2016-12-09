#include "document.h"
#include "document_p.h"

#include <memory>

DocumentPrivate::~DocumentPrivate()
{
}

void DocumentPrivate::init()
{
    Q_Q(Document);
    undoStack.reset(new QUndoStack);

    q->connect(undoStack.data(), &QUndoStack::canRedoChanged, q, &Document::canRedoChanged);
    q->connect(undoStack.data(), &QUndoStack::canUndoChanged, q, &Document::canUndoChanged);
}

/*!
    \class Document
    Base class for all documents.

    Provides base functionality like modified state or undo-redo stack.
*/

/*!
    Constructs an AbstractDocument with the given \a parent.
*/
Document::Document(QObject *parent) :
    QObject(parent),
    d_ptr(new DocumentPrivate(this))
{
    Q_D(Document);
    d->init();
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

/*!
    \property Document::canRedo
    This property holds whether there are any commands to redo.
*/

bool Document::canRedo() const
{
    Q_D(const Document);
    return d->undoStack->canRedo();
}

/*!
    \property Document::canUndo
    This property holds whether there are any commands to redo.
*/

bool Document::canUndo() const
{
    Q_D(const Document);
    return d->undoStack->canUndo();
}

/*!
    Clears the undo redo stack.
*/
void Document::clearUndoStack()
{
    Q_D(const Document);
    return d->undoStack->clear();
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
    Undoes the last operation.
*/
void Document::undo()
{
    Q_D(Document);
    d->undoStack->undo();
}

/*!
    Redoes the current operation.
*/
void Document::redo()
{
    Q_D(Document);
    d->undoStack->redo();
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

/*!
    Returns pointer to the undo-redo stack.
    Can be used to insert undo-redo commands.
*/
QUndoStack *Document::undoStack() const
{
    Q_D(const Document);
    return d->undoStack.data();
}


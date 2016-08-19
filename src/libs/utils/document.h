#pragma once

#include "utils_global.h"

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QUrl>
#include <QtCore/QIODevice>
#include <QtCore/QMimeType>
#include <QtCore/QVector>
#include <QtWidgets/QUndoStack>

class DocumentPrivate;
class UTILS_EXPORT Document : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Document)
    Q_DISABLE_COPY(Document)

    Q_PROPERTY(bool modified READ isModified WRITE setModified NOTIFY modificationChanged)
    Q_PROPERTY(bool canRedo READ canRedo NOTIFY canRedoChanged)
    Q_PROPERTY(bool canUndo READ canUndo NOTIFY canUndoChanged)
    // TODO: add undoRedoEnabled property

public:
    explicit Document(QObject *parent = Q_NULLPTR);
    ~Document();

    bool isModified() const;
    bool canRedo() const;
    bool canUndo() const;

public slots:
    void setModified(bool modified);

    void undo();
    void redo();

signals:
    void modificationChanged(bool modified);
    void canRedoChanged(bool available);
    void canUndoChanged(bool available);

protected:
    explicit Document(DocumentPrivate &dd, QObject *parent = Q_NULLPTR);

    QUndoStack *undoStack() const;

protected:
    QScopedPointer<DocumentPrivate> d_ptr;
};

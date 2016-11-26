#pragma once

#include "document.h"
#include <QtCore/QFile>

class UTILS_EXPORT DocumentPrivate
{
    Q_DECLARE_PUBLIC(Document)
    Q_DISABLE_COPY(DocumentPrivate)

protected:
    Document *q_ptr;

public:
    explicit DocumentPrivate(Document *qq) : q_ptr(qq) {}
    virtual ~DocumentPrivate();

    void init();

public:
    bool modified {false};
    QScopedPointer<QUndoStack> undoStack;
};

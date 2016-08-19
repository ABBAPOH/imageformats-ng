#ifndef DOCUMENT_P
#define DOCUMENT_P

#include "document.h"
#include <QtCore/QFile>

class DocumentPrivate
{
    Q_DECLARE_PUBLIC(Document)
    Q_DISABLE_COPY(DocumentPrivate)

protected:
    Document *q_ptr;

public:
    explicit DocumentPrivate(Document *qq) : q_ptr(qq) {}
    virtual ~DocumentPrivate() {}

    bool modified {false};
};

#endif // DOCUMENT_P


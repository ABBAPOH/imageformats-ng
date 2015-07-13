#ifndef ABSTRACTDOCUMENT_P
#define ABSTRACTDOCUMENT_P

#include "abstractdocument.h"
#include <QtCore/QFile>

class AbstractDocumentPrivate
{
    Q_DECLARE_PUBLIC(AbstractDocument)
    Q_DISABLE_COPY(AbstractDocumentPrivate)

public:
    explicit AbstractDocumentPrivate(AbstractDocument *qq);
    virtual ~AbstractDocumentPrivate() {}

    virtual void changed() {}

    QIODevice *device;
    QString fileName;
    QScopedPointer<QFile> file;
    QMimeType mimeType;

    bool modified;

protected:
    AbstractDocument *q_ptr;
};

#endif // ABSTRACTDOCUMENT_P


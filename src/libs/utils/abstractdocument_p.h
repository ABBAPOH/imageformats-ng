#ifndef ABSTRACTDOCUMENT_P
#define ABSTRACTDOCUMENT_P

#include "abstractdocument.h"
#include <QtCore/QFile>

class AbstractDocumentPrivate
{
    Q_DECLARE_PUBLIC(AbstractDocument)
    Q_DISABLE_COPY(AbstractDocumentPrivate)

protected:
    AbstractDocument *q_ptr;

public:
    explicit AbstractDocumentPrivate(AbstractDocument *qq) : q_ptr(qq) {}
    virtual ~AbstractDocumentPrivate() {}

    void setOpened(bool opened);

    QUrl url;

    bool opened {false};
    bool modified {false};

};

#endif // ABSTRACTDOCUMENT_P


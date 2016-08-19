#ifndef ABSTRACTDOCUMENT_H
#define ABSTRACTDOCUMENT_H

#include "utils_global.h"

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QUrl>
#include <QtCore/QIODevice>
#include <QtCore/QMimeType>
#include <QtCore/QVector>

class AbstractDocumentPrivate;
class UTILS_EXPORT AbstractDocument : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(AbstractDocument)
    Q_DISABLE_COPY(AbstractDocument)

    Q_PROPERTY(bool modified READ isModified WRITE setModified NOTIFY modificationChanged)

public:
    explicit AbstractDocument(QObject *parent = Q_NULLPTR);
    ~AbstractDocument();

    bool isModified() const;

public slots:
    void setModified(bool modified);

signals:
    void modificationChanged(bool modified);

protected:
    explicit AbstractDocument(AbstractDocumentPrivate &dd, QObject *parent = Q_NULLPTR);

protected:
    QScopedPointer<AbstractDocumentPrivate> d_ptr;
};

#endif // ABSTRACTDOCUMENT_H

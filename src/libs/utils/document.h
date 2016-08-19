#pragma once

#include "utils_global.h"

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtCore/QUrl>
#include <QtCore/QIODevice>
#include <QtCore/QMimeType>
#include <QtCore/QVector>

class DocumentPrivate;
class UTILS_EXPORT Document : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Document)
    Q_DISABLE_COPY(Document)

    Q_PROPERTY(bool modified READ isModified WRITE setModified NOTIFY modificationChanged)

public:
    explicit Document(QObject *parent = Q_NULLPTR);
    ~Document();

    bool isModified() const;

public slots:
    void setModified(bool modified);

signals:
    void modificationChanged(bool modified);

protected:
    explicit Document(DocumentPrivate &dd, QObject *parent = Q_NULLPTR);

protected:
    QScopedPointer<DocumentPrivate> d_ptr;
};

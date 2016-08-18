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

    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QMimeType mimeType READ mimeType WRITE setMimeType NOTIFY mimeTypeChanged)
    Q_PROPERTY(bool opened READ isOpened NOTIFY openedChanged)
    Q_PROPERTY(bool modified READ isModified WRITE setModified NOTIFY modificationChanged)

public:
    explicit AbstractDocument(QObject *parent = Q_NULLPTR);
    ~AbstractDocument();

    QUrl url() const;
    void setUrl(const QUrl &url);

    QMimeType mimeType() const;
    void setMimeType(const QMimeType &mimeType);
    void setMimeType(const QString &name);

    bool isOpened() const;
    bool isModified() const;

    virtual QVector<QMimeType> openableMimetypes() const = 0;
    virtual QVector<QMimeType> saveableMimetypes() const { return QVector<QMimeType>(); }

public slots:
    void open(const QVariantMap &options = QVariantMap());
    void save(const QVariantMap &options = QVariantMap());
    void setModified(bool modified);

signals:
    void urlChanged(const QUrl &url);
    void mimeTypeChanged(const QMimeType &mimeType);
    void modificationChanged(bool modified);
    void openedChanged(bool isOpened);
    void openFinished(bool ok);
    void saveFinished(bool ok);

protected:
    explicit AbstractDocument(AbstractDocumentPrivate &dd, QObject *parent = Q_NULLPTR);

    virtual void doOpen(const QUrl &url, const QVariantMap &options) = 0;
    virtual void doSave(const QUrl &url, const QVariantMap &options);
    void finishOpen(bool ok);
    void finishSave(bool ok);

protected:
    QScopedPointer<AbstractDocumentPrivate> d_ptr;
};

#endif // ABSTRACTDOCUMENT_H

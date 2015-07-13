#ifndef ABSTRACTDOCUMENT_H
#define ABSTRACTDOCUMENT_H

#include <QtCore/QObject>
#include <QtCore/QIODevice>
#include <QtCore/QMimeType>

class AbstractDocumentPrivate;
class AbstractDocument : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(AbstractDocument)
    Q_DISABLE_COPY(AbstractDocument)

    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(QMimeType mimeType READ mimeType WRITE setMimeType NOTIFY mimeTypeChanged)
    Q_PROPERTY(bool modified READ modified WRITE setModified NOTIFY modificationChanged)

public:
    explicit AbstractDocument(QObject *parent = Q_NULLPTR);
    ~AbstractDocument();

    QIODevice *device() const;
    void setDevice(QIODevice *device);

    QString fileName() const; // TODO: change to QUrl
    void setFileName(const QString &fileName);

    QMimeType mimeType() const;
    void setMimeType(const QMimeType &mimeType);
    void setMimeType(const QString &name);

    bool modified() const;

    virtual bool open() = 0;
    virtual bool save() { return false; }

public slots:
    void setModified(bool modified);

signals:
    void deviceChanged();
    void fileNameChanged(const QString &fileName);
    void mimeTypeChanged(const QMimeType &mimeType);
    void modificationChanged(bool modified);

protected:
    explicit AbstractDocument(AbstractDocumentPrivate &dd, QObject *parent = Q_NULLPTR);

protected:
    QScopedPointer<AbstractDocumentPrivate> d_ptr;
};

#endif // ABSTRACTDOCUMENT_H

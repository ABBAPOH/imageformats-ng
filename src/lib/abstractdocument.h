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

signals:
    void deviceChanged();
    void fileNameChanged(const QString &fileName);
    void mimeTypeChanged(const QMimeType &mimeType);

public slots:

protected:
    explicit AbstractDocument(AbstractDocumentPrivate &dd, QObject *parent = Q_NULLPTR);

protected:
    QScopedPointer<AbstractDocumentPrivate> d_ptr;
};

#endif // ABSTRACTDOCUMENT_H

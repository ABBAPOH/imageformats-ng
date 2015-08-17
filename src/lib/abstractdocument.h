#ifndef ABSTRACTDOCUMENT_H
#define ABSTRACTDOCUMENT_H

#include <QtCore/QObject>
#include <QtCore/QIODevice>
#include <QtCore/QMimeType>
#include <QtCore/QVector>

class AbstractDocumentPrivate;
class LIB_EXPORT AbstractDocument : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(AbstractDocument)
    Q_DISABLE_COPY(AbstractDocument)

    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    Q_PROPERTY(QMimeType mimeType READ mimeType WRITE setMimeType NOTIFY mimeTypeChanged)
    Q_PROPERTY(bool modified READ modified WRITE setModified NOTIFY modificationChanged)

public:
    class Result;

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

    Result open();
    Result save();

    virtual QVector<QMimeType> supportedInputMimetypes() const = 0;
    virtual QVector<QMimeType> supportedOutputMimetypes() const { return QVector<QMimeType>(); }

public slots:
    void setModified(bool modified);

signals:
    void deviceChanged();
    void fileNameChanged(const QString &fileName);
    void mimeTypeChanged(const QMimeType &mimeType);
    void modificationChanged(bool modified);

protected:
    explicit AbstractDocument(AbstractDocumentPrivate &dd, QObject *parent = Q_NULLPTR);

    virtual bool read() = 0;
    virtual bool write() { return false; }

protected:
    QScopedPointer<AbstractDocumentPrivate> d_ptr;
};

class LIB_EXPORT AbstractDocument::Result
{
public:
    enum ErrorCode
    {
        NoError,
        InvalidMimeTypeError,
        FileNotFoundError,
        DeviceError,
        UnsupportedMimeTypeError,
        IOError,
    };

    inline Result(ErrorCode errorCode = NoError) : _error(errorCode) {}
    inline ErrorCode errorCode() const { return _error; }
    QString errorString() const;

    operator bool() const { return _error == NoError; }

private:
    ErrorCode _error;
};





#endif // ABSTRACTDOCUMENT_H

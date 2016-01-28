#include <QtTest>

#include <AbstractDocument>
#include <QMimeDatabase>

class TestDocument : public AbstractDocument
{
public:
    explicit TestDocument(QObject *parent = Q_NULLPTR) : AbstractDocument(parent) {}

    QVector<QMimeType> supportedInputMimetypes() const Q_DECL_OVERRIDE
    {
        return QVector<QMimeType>() << QMimeDatabase().mimeTypeForName("text/plain");
    }

    QVector<QMimeType> supportedOutputMimetypes() const Q_DECL_OVERRIDE
    {
        return QVector<QMimeType>() << QMimeDatabase().mimeTypeForName("text/plain");
    }

    inline QString text() const { return _text; }
    void setText(const QString &text) { _text = text; }

protected:
    bool read() Q_DECL_OVERRIDE;
    bool write() Q_DECL_OVERRIDE;

private:
    QString _text;
};

bool TestDocument::read()
{
    const auto data = device()->readAll();
    _text = QString::fromUtf8(data);
    return true;
}

bool TestDocument::write()
{
    auto data = _text.toUtf8();
    return device()->write(data) == data.length();
}

class TestAbstractDocument : public QObject
{
    Q_OBJECT
private slots:
    void defaultValues();
    void setters();
    void read();
    void write();
};

void TestAbstractDocument::defaultValues()
{
    TestDocument doc;
    QVERIFY(doc.device() == Q_NULLPTR);
    QCOMPARE(doc.fileName(), QString());
    QCOMPARE(doc.mimeType(), QMimeType());
    QCOMPARE(doc.modified(), false);
}

void TestAbstractDocument::setters()
{
    QFile file("file.txt");
    file.remove();
    file.open(QIODevice::WriteOnly);
    file.write("Hello, world!");
    file.close();

    TestDocument doc;
    doc.setDevice(&file);
    QCOMPARE(doc.device(), &file);
    doc.setDevice(Q_NULLPTR);
    QVERIFY(doc.device() == Q_NULLPTR);

    doc.setFileName("file.txt");
    QCOMPARE(doc.fileName(), QString("file.txt"));
    QVERIFY(qobject_cast<QFile *>(doc.device()) != Q_NULLPTR);
    QCOMPARE(doc.mimeType().name(), QString("text/plain"));

    doc.setMimeType("text/html");
    QCOMPARE(doc.mimeType().name(), QString("text/html"));

    doc.setModified(true);
    QCOMPARE(doc.modified(), true);

    file.remove();
}

void TestAbstractDocument::read()
{
    QFile file("file.txt");
    file.remove();
    QVERIFY(file.open(QIODevice::WriteOnly));
    file.write("Hello, world!");
    file.close();

    TestDocument doc;
    auto ok = doc.open();
    QCOMPARE(ok.errorCode(), AbstractDocument::Result::InvalidMimeTypeError);

    doc.setMimeType("text/plain");
    ok = doc.open();
    QCOMPARE(ok.errorCode(), AbstractDocument::Result::DeviceError);

    doc.setDevice(&file);
    ok = doc.open();
    QCOMPARE(ok.errorCode(), AbstractDocument::Result::NoError);
    QCOMPARE(doc.text(), QStringLiteral("Hello, world!"));
    file.remove();
}

void TestAbstractDocument::write()
{
    TestDocument doc;
    doc.setText(QStringLiteral("Hello, world!"));

    auto ok = doc.save();
    QCOMPARE(ok.errorCode(), AbstractDocument::Result::InvalidMimeTypeError);

    doc.setMimeType("text/plain");

    ok = doc.save();
    QCOMPARE(ok.errorCode(), AbstractDocument::Result::DeviceError);

    QFile file("file.txt");
    file.remove();
    doc.setDevice(&file);

    ok = doc.save();
    QCOMPARE(ok.errorCode(), AbstractDocument::Result::NoError);
    file.close();

    QVERIFY(file.open(QIODevice::ReadOnly));
    QCOMPARE(file.readAll(), QByteArray("Hello, world!"));
}

QTEST_APPLESS_MAIN(TestAbstractDocument)

#include "tst_abstractdocument.moc"

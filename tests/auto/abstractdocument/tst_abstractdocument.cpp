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
    void doOpen() Q_DECL_OVERRIDE;
    void doSave() Q_DECL_OVERRIDE;

private:
    QString _text;
};

void TestDocument::doOpen()
{
    QVERIFY(url().isLocalFile());
    QFile file(url().toLocalFile());
    QVERIFY(file.open(QIODevice::ReadOnly));
    const auto data = file.readAll();
    _text = QString::fromUtf8(data);
    finishOpen(true);
}

void TestDocument::doSave()
{
    QVERIFY(url().isLocalFile());
    QFile file(url().toLocalFile());
    QVERIFY(file.open(QIODevice::WriteOnly));

    auto data = _text.toUtf8();
    const bool ok = file.write(data) == data.length();
    finishSave(ok);
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
    QCOMPARE(doc.url(), QUrl());
    QCOMPARE(doc.mimeType(), QMimeType());
    QCOMPARE(doc.isModified(), false);
}

void TestAbstractDocument::setters()
{
    QFile file("file.txt");
    file.remove();
    file.open(QIODevice::WriteOnly);
    file.write("Hello, world!");
    file.close();

    TestDocument doc;

    doc.setUrl(QUrl("file://file.txt"));
    QCOMPARE(doc.url(), QUrl("file://file.txt"));

    QCOMPARE(doc.mimeType().name(), QString("text/plain"));

    doc.setMimeType("text/html");
    QCOMPARE(doc.mimeType().name(), QString("text/html"));

    doc.setModified(true);
    QCOMPARE(doc.isModified(), true);

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
    doc.setUrl(QUrl::fromLocalFile("file.txt"));
    doc.open();
    QVERIFY(doc.isOpened());

    doc.setMimeType("text/plain");
    doc.open();
    QVERIFY(doc.isOpened());

    doc.open();
    QVERIFY(doc.isOpened());
    QCOMPARE(doc.text(), QStringLiteral("Hello, world!"));
    file.remove();
}

void TestAbstractDocument::write()
{
    TestDocument doc;
    doc.setUrl(QUrl::fromLocalFile("file.txt"));
    doc.setText(QStringLiteral("Hello, world!"));
    doc.save();

    QFile file("file.txt");
    QVERIFY(file.open(QIODevice::ReadOnly));
    QCOMPARE(file.readAll(), QByteArray("Hello, world!"));
}

QTEST_APPLESS_MAIN(TestAbstractDocument)

#include "tst_abstractdocument.moc"

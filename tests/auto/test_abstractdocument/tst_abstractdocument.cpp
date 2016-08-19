#include <QtTest>

#include <AbstractDocument>
#include <QMimeDatabase>

class TestDocument : public AbstractDocument
{
public:
    explicit TestDocument(QObject *parent = Q_NULLPTR) : AbstractDocument(parent) {}

    inline QString text() const { return _text; }
    void setText(const QString &text) { _text = text; }

    void open(const QUrl &url);
    void save(const QUrl &url);

private:
    QString _text;
};

void TestDocument::open(const QUrl &url)
{
    QVERIFY(url.isLocalFile());
    QFile file(url.toLocalFile());
    QVERIFY(file.open(QIODevice::ReadOnly));
    const auto data = file.readAll();
    _text = QString::fromUtf8(data);
}

void TestDocument::save(const QUrl &url)
{
    QVERIFY(url.isLocalFile());
    QFile file(url.toLocalFile());
    QVERIFY(file.open(QIODevice::WriteOnly));

    auto data = _text.toUtf8();
    const bool ok = file.write(data) == data.length();
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
    doc.open(QUrl::fromLocalFile("file.txt"));

    QCOMPARE(doc.text(), QStringLiteral("Hello, world!"));
    file.remove();
}

void TestAbstractDocument::write()
{
    TestDocument doc;
    doc.setText(QStringLiteral("Hello, world!"));
    doc.save(QUrl::fromLocalFile("file.txt"));

    QFile file("file.txt");
    QVERIFY(file.open(QIODevice::ReadOnly));
    QCOMPARE(file.readAll(), QByteArray("Hello, world!"));
}

QTEST_APPLESS_MAIN(TestAbstractDocument)

#include "tst_abstractdocument.moc"

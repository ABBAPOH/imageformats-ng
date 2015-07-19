#include <QtTest>

#include <ImageDocument>

class TestImageDocument : public QObject
{
    Q_OBJECT
private slots:
    void defaultValues();
    void setters();
};

void TestImageDocument::defaultValues()
{
    ImageDocument doc;

    QCOMPARE(doc.subType(), QByteArray());
    QCOMPARE(doc.contents(), ImageContents());

    ImageDocument doc2(QStringLiteral("image.png"));
    QCOMPARE(doc2.fileName(), QStringLiteral("image.png"));
}

void TestImageDocument::setters()
{
    ImageDocument doc;

    doc.setSubType("subType");
    QCOMPARE(doc.subType(), QByteArray("subType"));

    ImageContents c;
    c.setType(ImageContents::Cubemap);
    doc.setContents(c);
    QCOMPARE(doc.contents(), c);
}

QTEST_APPLESS_MAIN(TestImageDocument)

#include "tst_imagedocument.moc"

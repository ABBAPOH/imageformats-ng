#include <QtTest>

#include <ImageHeader>

class TestImageHeader : public QObject
{
    Q_OBJECT
private slots:
    void defaultValues();
    void setters();
    void equals();
};

void TestImageHeader::defaultValues()
{
    ImageHeader header;
    QCOMPARE(header.type(), ImageHeader::Image);
    QCOMPARE(header.size(), QSize());
    QCOMPARE(header.imageFormat(), QImage::Format_Invalid);
    QCOMPARE(header.name(), QString());
    QCOMPARE(header.imageCount(), 1);
    QCOMPARE(header.mipmapCount(), 0);
    QCOMPARE(header.loopCount(), -1);
}

void TestImageHeader::setters()
{
    ImageHeader header;

    header.setType(ImageHeader::Animation);
    QCOMPARE(header.type(), ImageHeader::Animation);

    header.setSize(QSize(64, 64));
    QCOMPARE(header.size(), QSize(64, 64));

    header.setImageFormat(QImage::Format_ARGB32);
    QCOMPARE(header.imageFormat(), QImage::Format_ARGB32);

    header.setName(QString("name"));
    QCOMPARE(header.name(), QString("name"));

    header.setImageCount(2);
    QCOMPARE(header.imageCount(), 2);

    header.setMipmapCount(2);
    QCOMPARE(header.mipmapCount(), 2);

    header.setLoopCount(10);
    QCOMPARE(header.loopCount(), 10);
}

void TestImageHeader::equals()
{
    ImageHeader c1;
    ImageHeader c2;

    QVERIFY(c1 == c2);

    c1.setSize(QSize(64, 64));
    QVERIFY(c1 != c2);

    c2.setSize(QSize(64, 64));
    QVERIFY(c1 == c2);

    c1.setName(QString("name"));
    QVERIFY(c1 != c2);
    c2.setName(QString("name"));
    QVERIFY(c1 == c2);

    c1.setImageCount(2);
    c2.setImageCount(2);

    QVERIFY(c1 == c2);
}

QTEST_APPLESS_MAIN(TestImageHeader)

#include "tst_imageheader.moc"

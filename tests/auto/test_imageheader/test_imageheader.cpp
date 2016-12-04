#include <QtTest>

#include <ImageHeader>

Q_DECLARE_METATYPE(ImageHeader::Type)
Q_DECLARE_METATYPE(QImage::Format)

class TestImageHeader : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void defaultValues();
    void setters();
    void equals();
    void validate_data();
    void validate();
};

void TestImageHeader::initTestCase()
{
    qRegisterMetaType<ImageHeader::Type>("ImageHeader::Type");
}

void TestImageHeader::defaultValues()
{
    ImageHeader header;
    QVERIFY(header.isNull());
    QCOMPARE(header.type(), ImageHeader::Invalid);
    QCOMPARE(header.width(), -1);
    QCOMPARE(header.height(), -1);
    QCOMPARE(header.depth(), -1);
    QCOMPARE(header.size(), QSize());
    QCOMPARE(header.imageFormat(), QImage::Format_Invalid);
    QCOMPARE(header.name(), QString());
    QCOMPARE(header.imageCount(), 1);
    QCOMPARE(header.mipmapCount(), 1);
    QCOMPARE(header.loopCount(), -1);
}

void TestImageHeader::setters()
{
    ImageHeader header;

    header.setType(ImageHeader::Image);
    QCOMPARE(header.type(), ImageHeader::Image);

    header.setSize(QSize(64, 64));
    QCOMPARE(header.size(), QSize(64, 64));

    header.setImageFormat(QImage::Format_ARGB32);
    QCOMPARE(header.imageFormat(), QImage::Format_ARGB32);

    header.setName(QString("name"));
    QCOMPARE(header.name(), QString("name"));

    header.setImageCount(2);
    QCOMPARE(header.imageCount(), 2);

    header.setHasMipmaps(true);
    QVERIFY(header.hasMipmaps());
    QCOMPARE(header.mipmapCount(), 7);

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

void TestImageHeader::validate_data()
{
    QTest::addColumn<ImageHeader::Type>("type");
    QTest::addColumn<QImage::Format>("imageFormat");
    QTest::addColumn<int>("width");
    QTest::addColumn<int>("heigth");
    QTest::addColumn<int>("depth");
    QTest::addColumn<bool>("valid");

    QTest::newRow("valid 1") << ImageHeader::Image << QImage::Format_ARGB32 << 1 << 1 << 1 << true;
    QTest::newRow("valid 2") << ImageHeader::Image << QImage::Format_ARGB32 << 1 << 1 << -1 << true;
    QTest::newRow("valid 3") << ImageHeader::VolumeTexture << QImage::Format_ARGB32 << 1 << 1 << 1 << true;
    QTest::newRow("invalid type") << ImageHeader::Invalid << QImage::Format_ARGB32 << 1 << 1 << 1 << false;
    QTest::newRow("invalid format") << ImageHeader::Image << QImage::Format_Invalid << 1 << 1 << 1 << false;
    QTest::newRow("invalid width") << ImageHeader::Image << QImage::Format_ARGB32 << 0 << 1 << 1 << false;
    QTest::newRow("invalid heigth") << ImageHeader::Image << QImage::Format_ARGB32 << 1 << 0 << 1 << false;
    QTest::newRow("invalid depth") << ImageHeader::VolumeTexture << QImage::Format_ARGB32 << 1 << 1 << 0 << false;
}

void TestImageHeader::validate()
{
    QFETCH(ImageHeader::Type, type);
    QFETCH(QImage::Format, imageFormat);
    QFETCH(int, width);
    QFETCH(int, heigth);
    QFETCH(int, depth);
    QFETCH(bool, valid);

    ImageHeader header;
    header.setType(type);
    header.setImageFormat(imageFormat);
    header.setWidth(width);
    header.setHeight(heigth);
    header.setDepth(depth);

    QString error;
    QVERIFY2(valid == header.validate(&error), qPrintable(error));
}

QTEST_APPLESS_MAIN(TestImageHeader)

#include "test_imageheader.moc"

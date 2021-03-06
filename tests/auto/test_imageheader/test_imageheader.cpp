#include <QtTest>

#include <ImageHeader>

Q_DECLARE_METATYPE(ImageResource::Type)
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
    qRegisterMetaType<ImageResource::Type>("ImageResource::Type");
}

void TestImageHeader::defaultValues()
{
    ImageHeader header;
    QVERIFY(header.isNull());
    QCOMPARE(header.type(), ImageResource::Type::Invalid);
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

    header.setType(ImageResource::Type::Image);
    QCOMPARE(header.type(), ImageResource::Type::Image);

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

    header.setFrameDelay(10);
    QCOMPARE(header.frameDelay(), 10);

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
    QTest::addColumn<ImageResource::Type>("type");
    QTest::addColumn<QImage::Format>("imageFormat");
    QTest::addColumn<int>("width");
    QTest::addColumn<int>("heigth");
    QTest::addColumn<int>("depth");
    QTest::addColumn<int>("imageCount");
    QTest::addColumn<int>("frameDelay");
    QTest::addColumn<int>("loopCount");
    QTest::addColumn<bool>("valid");

    QTest::newRow("valid 1") << ImageResource::Type::Image << QImage::Format_ARGB32 << 1 << 1 << 1 << 1 << 0 << -1 << true;
    QTest::newRow("valid 2") << ImageResource::Type::Image << QImage::Format_ARGB32 << 1 << 1 << -1 << 1 << 0 << -1 << true;
    QTest::newRow("valid 3") << ImageResource::Type::VolumeTexture << QImage::Format_ARGB32 << 1 << 1 << 1 << 1 << 0 << -1 << true;
    QTest::newRow("invalid type") << ImageResource::Type::Invalid << QImage::Format_ARGB32 << 1 << 1 << 1 << 1 << 0 << -1 << false;
    QTest::newRow("invalid format") << ImageResource::Type::Image << QImage::Format_Invalid << 1 << 1 << 1 << 1 << 0 << -1 << false;
    QTest::newRow("invalid width") << ImageResource::Type::Image << QImage::Format_ARGB32 << 0 << 1 << 1 << 1 << 0 << -1 << false;
    QTest::newRow("invalid heigth") << ImageResource::Type::Image << QImage::Format_ARGB32 << 1 << 0 << 1 << 1 << 0 << -1 << false;
    QTest::newRow("invalid depth") << ImageResource::Type::VolumeTexture << QImage::Format_ARGB32 << 1 << 1 << 0 << 1 << 0 << -1 << false;
    QTest::newRow("invalid image count") << ImageResource::Type::VolumeTexture << QImage::Format_ARGB32 << 1 << 1 << 1 << 0 << 0 << -1 << false;
    QTest::newRow("invalid frame delay") << ImageResource::Type::VolumeTexture << QImage::Format_ARGB32 << 1 << 1 << 1 << 1 << -1 << -1 << false;
    QTest::newRow("invalid loop count") << ImageResource::Type::VolumeTexture << QImage::Format_ARGB32 << 1 << 1 << 1 << 1 << 0 << -2 << false;
}

void TestImageHeader::validate()
{
    QFETCH(ImageResource::Type, type);
    QFETCH(QImage::Format, imageFormat);
    QFETCH(int, width);
    QFETCH(int, heigth);
    QFETCH(int, depth);
    QFETCH(int, imageCount);
    QFETCH(int, frameDelay);
    QFETCH(int, loopCount);
    QFETCH(bool, valid);

    ImageHeader header;
    header.setType(type);
    header.setImageFormat(imageFormat);
    header.setWidth(width);
    header.setHeight(heigth);
    header.setDepth(depth);
    header.setImageCount(imageCount);
    header.setFrameDelay(frameDelay);
    header.setLoopCount(loopCount);

    QString error;
    QVERIFY2(valid == header.validate(&error), qPrintable(error));
}

QTEST_APPLESS_MAIN(TestImageHeader)

#include "test_imageheader.moc"

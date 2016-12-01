#include <QtTest>

#include <ImageHeader>

class TestImageHeader : public QObject
{
    Q_OBJECT
private slots:
    void defaultValues();
    void setters();
    void equals();
    void validate();
};

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

void TestImageHeader::validate()
{
    ImageHeader valid;
    valid.setType(ImageHeader::Image);
    valid.setImageFormat(QImage::Format_ARGB32);
    valid.setSize(QSize(64, 64));
    QString error;
    QVERIFY2(valid.validate(&error), error.toLocal8Bit().data());

    ImageHeader invalid;
    QVERIFY(!invalid.validate(&error));

    invalid = valid;
    invalid.setType(ImageHeader::Invalid);
    QVERIFY(!invalid.validate(&error));

    invalid = valid;
    invalid.setImageFormat(QImage::Format_Invalid);
    QVERIFY(!invalid.validate(&error));

    invalid = valid;
    invalid.setWidth(-100);
    QVERIFY(!invalid.validate(&error));

    invalid = valid;
    invalid.setHeigth(-100);
    QVERIFY(!invalid.validate(&error));

    invalid = valid;
    invalid.setType(ImageHeader::VolumeTexture);
    invalid.setDepth(-100);
    QVERIFY(!invalid.validate(&error));

    invalid = valid;
    invalid.setImageCount(-20);
    QVERIFY(!invalid.validate(&error));

    invalid = valid;
    invalid.setFrameDelay(-1);
    QVERIFY(!invalid.validate(&error));

    invalid = valid;
    invalid.setLoopCount(-2);
    QVERIFY(!invalid.validate(&error));
}

QTEST_APPLESS_MAIN(TestImageHeader)

#include "test_imageheader.moc"

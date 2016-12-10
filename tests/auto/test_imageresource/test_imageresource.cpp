#include <QtTest>
#include <ImageResource>

class TestImageResource : public QObject
{
    Q_OBJECT
private slots:
    void defaultConstructed();
    void image();
    void cubeTexture();
    void volumeTexture();
};

void TestImageResource::defaultConstructed()
{
    ImageResource res;
    QVERIFY(res.isNull());
    QCOMPARE(res.type(), ImageResource::Type::Invalid);
    QVERIFY(res.image().isNull());
    QVERIFY(res.cubeTexture().isNull());
    QVERIFY(res.volumeTexture().isNull());
}

void TestImageResource::image()
{
    QImage image(64, 64, QImage::Format_ARGB32);
    image.fill(Qt::white);

    ImageResource res(image);
    QVERIFY(!res.isNull());
    QCOMPARE(res.type(), ImageResource::Type::Image);
    QCOMPARE(res.image(), image);
    QVERIFY(res.cubeTexture().isNull());
    QVERIFY(res.volumeTexture().isNull());
}

void TestImageResource::cubeTexture()
{
    QImage image(64, 64, QImage::Format_ARGB32);
    image.fill(Qt::white);

    CubeTexture tex(64, QImage::Format_ARGB32);
    tex.setSide(CubeTexture::Side::PositiveX, image);
    tex.setSide(CubeTexture::Side::PositiveY, image);
    tex.setSide(CubeTexture::Side::PositiveZ, image);
    tex.setSide(CubeTexture::Side::NegativeX, image);
    tex.setSide(CubeTexture::Side::NegativeY, image);
    tex.setSide(CubeTexture::Side::NegativeZ, image);

    ImageResource res(tex);
    QVERIFY(!res.isNull());
    QCOMPARE(res.type(), ImageResource::Type::CubeTexture);
    QVERIFY(res.image().isNull());
    QCOMPARE(res.cubeTexture(), tex);
    QVERIFY(res.volumeTexture().isNull());
}

void TestImageResource::volumeTexture()
{
    QImage image(64, 64, QImage::Format_ARGB32);
    image.fill(Qt::white);

    QVector<QImage> images;
    images.resize(64);
    std::fill(images.begin(), images.end(), image);

    VolumeTexture tex(images);

    ImageResource res(tex);
    QVERIFY(!res.isNull());
    QCOMPARE(res.type(), ImageResource::Type::VolumeTexture);
    QVERIFY(res.image().isNull());
    QVERIFY(res.cubeTexture().isNull());
    QCOMPARE(res.volumeTexture(), tex);
}

QTEST_MAIN(TestImageResource)

#include "test_imageresource.moc"

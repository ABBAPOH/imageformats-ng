#include <QtTest>

#include <ImageContents>
#include <ImageExifMeta>

class TestImageContents : public QObject
{
    Q_OBJECT
private slots:
    void defaultValues();
    void constructFromImage();
    void constructFromCubeTexture();
    void constructFromVolumeTexture();
    void setters();
    void equals_data();
    void equals();
};

void TestImageContents::defaultValues()
{
    ImageContents contents;
    QCOMPARE(contents.exifMeta(), ImageExifMeta());
    QCOMPARE(contents.image(), QImage());
}

void TestImageContents::constructFromImage()
{
    auto format = QImage::Format_ARGB32;
    QImage image(32, 64, format);
    image.fill(Qt::white);

    ImageContents res(image);
    QVERIFY(!res.isNull());
    QCOMPARE(res.header().type(), ImageResource::Type::Image);
    QCOMPARE(res.header().width(), 32);
    QCOMPARE(res.header().height(), 64);
    QCOMPARE(res.header().depth(), 1);
    QCOMPARE(res.header().imageFormat(), format);
    QCOMPARE(res.image(), image);
}

void TestImageContents::constructFromCubeTexture()
{
    auto format = QImage::Format_ARGB32;
    CubeTexture texture(64, format);

    ImageContents res(texture);
    QVERIFY(!res.isNull());
    QCOMPARE(res.header().type(), ImageResource::Type::CubeTexture);
    QCOMPARE(res.header().width(), 64);
    QCOMPARE(res.header().height(), 64);
    QCOMPARE(res.header().depth(), 64);
    QCOMPARE(res.header().imageFormat(), format);
    QCOMPARE(res.resource().cubeTexture(), texture);
}

void TestImageContents::constructFromVolumeTexture()
{
    auto format = QImage::Format_ARGB32;
    VolumeTexture texture(16, 32, 64, format);
    texture.fill(Qt::white);

    ImageContents res(texture);
    QVERIFY(!res.isNull());
    QCOMPARE(res.header().type(), ImageResource::Type::VolumeTexture);
    QCOMPARE(res.header().width(), 16);
    QCOMPARE(res.header().height(), 32);
    QCOMPARE(res.header().depth(), 64);
    QCOMPARE(res.header().imageFormat(), format);
    QCOMPARE(res.resource().volumeTexture(), texture);
}

void TestImageContents::setters()
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
    QCOMPARE(header.mipmapCount(), 7);

    header.setLoopCount(10);
    QCOMPARE(header.loopCount(), 10);

    ImageContents contents(header);

    for (int i = 0; i < header.imageCount(); ++i) {
        for (int j = 0; j < header.mipmapCount(); ++j) {
            int size = 1 << (header.mipmapCount() - j - 1);
            QImage image(size, size, QImage::Format_ARGB32);
            const int red = 255 * i / (header.imageCount() - 1);
            const int green = 255 * j / (header.mipmapCount() - 1);
            const int blue = 255 * (i + j)
                    / (header.imageCount() + header.mipmapCount() - 2);
            image.fill(QColor(red, green, blue));
            contents.setImage(image, i, j);
            QCOMPARE(contents.image(i, j), image);
        }
    }
}

void TestImageContents::equals_data()
{
    QTest::addColumn<QSize>("size");
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("imageCount");

    QTest::newRow("") << QSize(64, 64) << QString("name") << 2;
}

void TestImageContents::equals()
{
    QFETCH(QSize, size);
    QFETCH(QString, name);
    QFETCH(int, imageCount);

    ImageHeader h;
    h.setType(ImageResource::Type::Image);
    h.setImageFormat(QImage::Format_ARGB32);
    h.setSize(size);
    h.setName(name);
    h.setImageCount(imageCount);

    ImageContents c1;
    ImageContents c2;

    QVERIFY(c1 == c2);

    c1 = ImageContents(h);
    c2 = ImageContents(h);
    QVERIFY(!c1.isNull());
    QVERIFY(!c2.isNull());

    for (int i = 0; i < imageCount; ++i) {
        QImage image(size, QImage::Format_ARGB32);
        image.fill(QColor(255 * i / 2, 255 * i / 2, 255 * (2*i) / 4));
        c1.setImage(image, i);
    }

    QVERIFY(c1 != c2);

    for (int i = 0; i < imageCount; ++i) {
        QImage image(size, QImage::Format_ARGB32);
        image.fill(QColor(255 * i / 2, 255 * i / 2, 255 * (2*i) / 4));
        c2.setImage(image, i);
    }

    QVERIFY(c1 == c2);
}

QTEST_APPLESS_MAIN(TestImageContents)

#include "tst_imagecontents.moc"

#include <QtTest>

#include "testhandler.h"
#include "testhandler_p.h"
#include <ImageDocument>

#include <private/ImageIOHandlerDatabase>

class TestImageDocument : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void defaultValues();
    void setters();
    void handlerLoaded();
    void read_data();
    void read();
    void write_data();
    void write();
    void writeOptions_data();
    void writeOptions();
};

void TestImageDocument::initTestCase()
{
}

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

void TestImageDocument::handlerLoaded()
{
    const auto mimeType = QMimeDatabase().mimeTypeForName("application/octet-stream");
    QVERIFY(mimeType.isValid());
    ImageDocument doc;
    QVERIFY(doc.supportedInputMimetypes().contains(mimeType));
    QVERIFY(doc.supportedOutputMimetypes().contains(mimeType));
}

void TestImageDocument::read_data()
{
    QTest::addColumn<int>("type");
    QTest::addColumn<QSize>("size");
    QTest::addColumn<int>("imageFormat");
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("imageCount");
    QTest::addColumn<int>("mipmapCount");
    QTest::addColumn<int>("loopCount");

//    QTest::newRow("0x0, AGRB32, 1x1, -1") << int(ImageContents::Image) << QSize() << int(QImage::Format_ARGB32) << QString() << 1 << 1 << -1;
    QTest::newRow("1x1, AGRB32, 1x1, -1") << int(ImageContents::Image) << QSize(1, 1) << int(QImage::Format_ARGB32) << QString() << 1 << 1 << -1;
    QTest::newRow("64x64, AGRB32, 1x1, -1") << int(ImageContents::Image) << QSize(1, 1) << int(QImage::Format_ARGB32) << QString() << 1 << 1 << -1;
    QTest::newRow("64x64, GRB32, 1x1, -1") << int(ImageContents::Image) << QSize(1, 1) << int(QImage::Format_RGB32) << QString() << 1 << 1 << -1;
    QTest::newRow("64x64, invalid, 1x1, -1") << int(ImageContents::Image) << QSize(1, 1) << int(QImage::Format_Invalid) << QString() << 1 << 1 << -1;
    QTest::newRow("image1x1") << int(ImageContents::Image) << QSize(64, 64) << int(QImage::Format_ARGB32) << QString() << 1 << 1 << -1;
    QTest::newRow("image1x2") << int(ImageContents::Image) << QSize(64, 64) << int(QImage::Format_ARGB32) << QString() << 1 << 2 << -1;
    QTest::newRow("image1x4") << int(ImageContents::Image) << QSize(64, 64) << int(QImage::Format_ARGB32) << QString() << 1 << 4 << -1;
    QTest::newRow("image2x1") << int(ImageContents::Image) << QSize(64, 64) << int(QImage::Format_ARGB32) << QString() << 2 << 1 << -1;
    QTest::newRow("image4x1") << int(ImageContents::Image) << QSize(64, 64) << int(QImage::Format_ARGB32) << QString() << 4 << 1 << -1;
    QTest::newRow("image4x4") << int(ImageContents::Image) << QSize(64, 64) << int(QImage::Format_ARGB32) << QString() << 4 << 4 << -1;
    QTest::newRow("named image1x1") << int(ImageContents::Image) << QSize(64, 64) << int(QImage::Format_ARGB32) << QString("name") << 1 << 1 << -1;
    QTest::newRow("image1x1 loop count = 1") << int(ImageContents::Image) << QSize(1, 1) << int(QImage::Format_ARGB32) << QString() << 1 << 1 << 1;
}

static QVector<QImage> generateImages(QSize size, int imageCount, int mipmapCount, QImage::Format format)
{
    if (format == QImage::Format_Invalid)
        format = QImage::Format_ARGB32;
    const auto generateImage = [=](int index, int level) {

        const int divisor = 1 << level;
        QSize mipmapSize = QSize(size.width() / divisor, size.height() / divisor);
        QImage result(mipmapSize, format);

        const auto generatePixel = [=](int x, int y) {
            return qRgba(x * 255.0 / mipmapSize.width(),
                         y * 255.0 / mipmapSize.height(),
                         index * 255.0 / imageCount,
                         level * 255.0 / mipmapCount);
        };

        for (int w = 0; w < mipmapSize.width(); w++) {
            for (int h = 0; h < mipmapSize.height(); ++h) {
                result.setPixel(w, h, generatePixel(w, h));
            }
        }

        return result;

    };

    QVector<QImage> images;
    for (int level = 0; level < mipmapCount; ++level) {
        for (int index = 0; index < imageCount; ++index) {
            images.append(generateImage(index, level));
        }
    }

    return images;
}

void TestImageDocument::read()
{
    QFETCH(int, type);
    QFETCH(QSize, size);
    QFETCH(int, imageFormat);
    QFETCH(QString, name);
    QFETCH(int, imageCount);
    QFETCH(int, mipmapCount);
    QFETCH(int, loopCount);

    TestImageData data;
    data.type = ImageContents::Type(type);
    data.imageFormat = QImage::Format(imageFormat);
    data.size = size;
    data.name = name;
    data.imageCount = imageCount;
    data.mipmapCount = mipmapCount;
    data.loopCount = loopCount;
    data.images = generateImages(size, imageCount, mipmapCount, data.imageFormat);

    QBuffer buffer;
    QVERIFY(buffer.open(QIODevice::ReadWrite));
    QDataStream stream(&buffer);
    stream << data;
    buffer.seek(0);

    ImageDocument doc;
    doc.setDevice(&buffer);
    doc.setMimeType("application/octet-stream");
    const auto ok = doc.open();
    QVERIFY2(ok, ok.errorString().toUtf8().constData());

    const auto contents = doc.contents();
    QCOMPARE(contents.size(), size);
    if (imageFormat != QImage::Format_Invalid)
        QCOMPARE(int(contents.imageFormat()), imageFormat);
    else
        QCOMPARE(contents.imageFormat(), QImage::Format_ARGB32); // fallback in generateImage
    QCOMPARE(contents.name(), name);
    QCOMPARE(contents.imageCount(), imageCount);
    QCOMPARE(contents.mipmapCount(), mipmapCount);
    QCOMPARE(contents.loopCount(), loopCount);

    for (int level = 0, i = 0; level < mipmapCount; ++level) {
        for (int index = 0; index < imageCount; ++index, i++) {
            QCOMPARE(contents.image(index, level), data.images[i]);
        }
    }
}

void TestImageDocument::write_data()
{
    QTest::addColumn<int>("type");
    QTest::addColumn<QSize>("size");
    QTest::addColumn<int>("imageFormat");
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("imageCount");
    QTest::addColumn<int>("mipmapCount");
    QTest::addColumn<int>("loopCount");

    QTest::newRow("1x1, AGRB32, 1x1, -1") << int(ImageContents::Image) << QSize(1, 1) << int(QImage::Format_ARGB32) << QString() << 1 << 1 << -1;
    QTest::newRow("64x64, AGRB32, 1x1, -1") << int(ImageContents::Image) << QSize(1, 1) << int(QImage::Format_ARGB32) << QString() << 1 << 1 << -1;
    QTest::newRow("64x64, GRB32, 1x1, -1") << int(ImageContents::Image) << QSize(1, 1) << int(QImage::Format_RGB32) << QString() << 1 << 1 << -1;
    QTest::newRow("64x64, invalid, 1x1, -1") << int(ImageContents::Image) << QSize(1, 1) << int(QImage::Format_Invalid) << QString() << 1 << 1 << -1;
    QTest::newRow("image1x1") << int(ImageContents::Image) << QSize(64, 64) << int(QImage::Format_ARGB32) << QString() << 1 << 1 << -1;
    QTest::newRow("image1x2") << int(ImageContents::Image) << QSize(64, 64) << int(QImage::Format_ARGB32) << QString() << 1 << 2 << -1;
    QTest::newRow("image1x4") << int(ImageContents::Image) << QSize(64, 64) << int(QImage::Format_ARGB32) << QString() << 1 << 4 << -1;
    QTest::newRow("image2x1") << int(ImageContents::Image) << QSize(64, 64) << int(QImage::Format_ARGB32) << QString() << 2 << 1 << -1;
    QTest::newRow("image4x1") << int(ImageContents::Image) << QSize(64, 64) << int(QImage::Format_ARGB32) << QString() << 4 << 1 << -1;
    QTest::newRow("image4x4") << int(ImageContents::Image) << QSize(64, 64) << int(QImage::Format_ARGB32) << QString() << 4 << 4 << -1;
    QTest::newRow("named image1x1") << int(ImageContents::Image) << QSize(64, 64) << int(QImage::Format_ARGB32) << QString("name") << 1 << 1 << -1;
    QTest::newRow("image1x1 loop count = 1") << int(ImageContents::Image) << QSize(1, 1) << int(QImage::Format_ARGB32) << QString() << 1 << 1 << 1;
}

void TestImageDocument::write()
{
    QFETCH(int, type);
    QFETCH(QSize, size);
    QFETCH(int, imageFormat);
    QFETCH(QString, name);
    QFETCH(int, imageCount);
    QFETCH(int, mipmapCount);
    QFETCH(int, loopCount);

    QBuffer buffer;
    QVERIFY(buffer.open(QIODevice::ReadWrite));

    ImageDocument doc;
    doc.setDevice(&buffer);
    doc.setMimeType("application/octet-stream");

    ImageContents contents;
    contents.setType(ImageContents::Type(type));
    contents.setSize(size);
    contents.setImageFormat(QImage::Format(imageFormat));
    contents.setName(name);
    contents.setImageCount(imageCount);
    contents.setMipmapCount(mipmapCount);
    contents.setLoopCount(loopCount);

    auto images = generateImages(size, imageCount, mipmapCount, QImage::Format(imageFormat));

    for (int level = 0, i = 0; level < mipmapCount; ++level) {
        for (int index = 0; index < imageCount; ++index, i++) {
            contents.setImage(images[i], index, level);
        }
    }
    doc.setContents(contents);

    const auto ok = doc.save();
    QVERIFY2(ok, ok.errorString().toUtf8().constData());

    TestImageData data;
    buffer.seek(0);
    QDataStream stream(&buffer);
    stream >> data;

    QCOMPARE(data.type, ImageContents::Type(type));
    QCOMPARE(data.size, size);
    if (imageFormat != QImage::Format_Invalid)
        QCOMPARE(data.imageFormat, QImage::Format(imageFormat));
    else
        QCOMPARE(data.imageFormat, QImage::Format_ARGB32); // fallback in generateImage
    QCOMPARE(data.name, name);
    QCOMPARE(data.imageCount, imageCount);
    QCOMPARE(data.mipmapCount, mipmapCount);
    QCOMPARE(data.loopCount, loopCount);

    for (int level = 0, i = 0; level < mipmapCount; ++level) {
        for (int index = 0; index < imageCount; ++index, i++) {
            QCOMPARE(data.images[i], images[i]);
        }
    }
}

void TestImageDocument::writeOptions_data()
{
    QTest::addColumn<int>("compression");
    QTest::addColumn<int>("gamma");
    QTest::addColumn<int>("quality");

    QTest::newRow("empty") << -1 << -1 << -1;
    QTest::newRow("compression") << 100 << -1 << -1;
    QTest::newRow("gamma") << -1 << 100 << -1;
    QTest::newRow("quality") << -1 << -1 << 100;
    QTest::newRow("all") << 100 << 100 << 100;
    QTest::newRow("all-half") << 50 << 50 << 50;
}

void TestImageDocument::writeOptions()
{
    QFETCH(int, compression);
    QFETCH(int, gamma);
    QFETCH(int, quality);

    const int imageCount = 1;
    const int mipmapCount = 1;
    const QSize size(64, 64);
    const QImage::Format imageFormat(QImage::Format_ARGB32);

    ImageContents contents;
    contents.setType(ImageContents::Image);
    contents.setSize(size);

    auto images = generateImages(size, imageCount, mipmapCount, QImage::Format(imageFormat));

    for (int level = 0, i = 0; level < mipmapCount; ++level) {
        for (int index = 0; index < imageCount; ++index, i++) {
            contents.setImage(images[i], index, level);
        }
    }

    QBuffer buffer;
    QVERIFY(buffer.open(QIODevice::ReadWrite));

    ImageDocument doc;
    doc.setDevice(&buffer);
    doc.setMimeType("application/octet-stream");
    doc.setContents(contents);

    WriteOptions options;
    if (compression != -1)
        options.setCompression(compression);
    if (gamma != -1)
        options.setGamma(gamma);
    if (quality != -1)
        options.setQuality(quality);

    const auto ok = doc.save(options);
    QVERIFY2(ok, ok.errorString().toUtf8().constData());

    TestImageData data;
    buffer.seek(0);
    QDataStream stream(&buffer);
    stream >> data;

    QCOMPARE(data.compression, compression);
    QCOMPARE(data.gamma, gamma);
    QCOMPARE(data.quality, quality);
}

QTEST_APPLESS_MAIN(TestImageDocument)

#include "tst_imagedocument.moc"

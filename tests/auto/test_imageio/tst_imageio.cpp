#include <QtTest>

#include "testhandler.h"
#include "testhandler_p.h"
#include <ImageIO>

#include <private/ImageIOHandlerDatabase>

#include <QtCore/QMimeType>

class TestImageIO : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void defaultValues();
    void setters();
    void read_data();
    void read();
    void write_data();
    void write();
    void writeOptions_data();
    void writeOptions();
};

void TestImageIO::initTestCase()
{
}

void TestImageIO::defaultValues()
{
    ImageIO io;

    QCOMPARE(io.subType(), QByteArray());

    ImageIO io2(QStringLiteral("image.png"));
    QCOMPARE(io2.fileName(), QStringLiteral("image.png"));
}

void TestImageIO::setters()
{
    QFile file("file.txt");
    file.remove();
    file.open(QIODevice::WriteOnly);
    file.write("Hello, world!");
    file.close();

    ImageIO io;

    io.setDevice(&file);
    QCOMPARE(io.device(), &file);
    io.setDevice(nullptr);
    QVERIFY(io.device() == nullptr);

    io.setFileName("file.txt");
    QCOMPARE(io.fileName(), QString("file.txt"));
    QVERIFY(qobject_cast<QFile *>(io.device()) != nullptr);
    QCOMPARE(io.mimeType().name(), QString("text/plain"));

    io.setMimeType(QMimeDatabase().mimeTypeForName("text/html"));
    QCOMPARE(io.mimeType().name(), QString("text/html"));

    io.setSubType("subType");
    QCOMPARE(io.subType(), QByteArray("subType"));
}

void TestImageIO::read_data()
{
    QTest::addColumn<int>("type");
    QTest::addColumn<QSize>("size");
    QTest::addColumn<int>("imageFormat");
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("imageCount");
    QTest::addColumn<int>("mipmapCount");
    QTest::addColumn<int>("loopCount");

//    QTest::newRow("0x0, AGRB32, 1x1, -1") << int(ImageHeader::Image) << QSize() << int(QImage::Format_ARGB32) << QString() << 1 << 1 << -1;
    QTest::newRow("1x1, AGRB32, 1x1, -1") << int(ImageResource::Type::Image) << QSize(1, 1) << int(QImage::Format_ARGB32) << QString() << 1 << 1 << -1;
    QTest::newRow("64x64, AGRB32, 1x1, -1") << int(ImageResource::Type::Image) << QSize(1, 1) << int(QImage::Format_ARGB32) << QString() << 1 << 1 << -1;
    QTest::newRow("64x64, GRB32, 1x1, -1") << int(ImageResource::Type::Image) << QSize(1, 1) << int(QImage::Format_RGB32) << QString() << 1 << 1 << -1;
//    QTest::newRow("64x64, invalid, 1x1, -1") << int(ImageHeader::Image) << QSize(1, 1) << int(QImage::Format_Invalid) << QString() << 1 << 1 << -1;
    QTest::newRow("image1x1") << int(ImageResource::Type::Image) << QSize(1, 1) << int(QImage::Format_ARGB32) << QString() << 1 << 1 << -1;
    QTest::newRow("image1x2") << int(ImageResource::Type::Image) << QSize(2, 2) << int(QImage::Format_ARGB32) << QString() << 1 << 2 << -1;
    QTest::newRow("image1x4") << int(ImageResource::Type::Image) << QSize(8, 8) << int(QImage::Format_ARGB32) << QString() << 1 << 4 << -1;
    QTest::newRow("image2x1") << int(ImageResource::Type::Image) << QSize(1, 1) << int(QImage::Format_ARGB32) << QString() << 2 << 1 << -1;
    QTest::newRow("image4x1") << int(ImageResource::Type::Image) << QSize(1, 1) << int(QImage::Format_ARGB32) << QString() << 4 << 1 << -1;
    QTest::newRow("image4x4") << int(ImageResource::Type::Image) << QSize(8, 8) << int(QImage::Format_ARGB32) << QString() << 4 << 4 << -1;
    QTest::newRow("named image1x1") << int(ImageResource::Type::Image) << QSize(64, 64) << int(QImage::Format_ARGB32) << QString("name") << 1 << 1 << -1;
    QTest::newRow("image1x1 loop count = 1") << int(ImageResource::Type::Image) << QSize(1, 1) << int(QImage::Format_ARGB32) << QString() << 1 << 1 << 1;
}

static QVector<QImage> generateImages(QSize size, int imageCount, int mipmapCount, QImage::Format format)
{
    if (format == QImage::Format_Invalid)
        format = QImage::Format_ARGB32;
    const auto generateImage = [=](int index, int level) {

//        const int divisor = 1 << level;
//        QSize mipmapSize = QSize(size.width() / divisor, size.height() / divisor);
        QSize mipmapSize = QSize(std::max(1, size.width() >> level), std::max(1, size.height() >> level));
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

void TestImageIO::read()
{
    QFETCH(int, type);
    QFETCH(QSize, size);
    QFETCH(int, imageFormat);
    QFETCH(QString, name);
    QFETCH(int, imageCount);
    QFETCH(int, mipmapCount);
    QFETCH(int, loopCount);

    TestImageData data;
    data.type = ImageResource::Type(type);
    data.imageFormat = QImage::Format(imageFormat);
    data.size = size;
    data.name = name;
    data.imageCount = imageCount;
    data.hasMipmaps = mipmapCount > 1;
    data.loopCount = loopCount;
    data.images = generateImages(size, imageCount, mipmapCount, data.imageFormat);

    QBuffer buffer;
    QVERIFY(buffer.open(QIODevice::WriteOnly));
    QDataStream stream(&buffer);
    stream << data;
    buffer.close(); // close device to ensure ImageIO will open it with correct mode

    ImageIO io;
    io.setDevice(&buffer);
    io.setMimeType("application/octet-stream");
    const auto maybeContents = io.read();
    QVERIFY(maybeContents);
    auto ok = io.error();
    QVERIFY2(ok, ok.errorString().toUtf8().constData());

    const auto contents = *maybeContents;
    const auto header = contents.header();
    QCOMPARE(header.size(), size);
    if (imageFormat != QImage::Format_Invalid)
        QCOMPARE(int(header.imageFormat()), imageFormat);
    else
        QCOMPARE(header.imageFormat(), QImage::Format_ARGB32); // fallback in generateImage
    QCOMPARE(header.name(), name);
    QCOMPARE(header.imageCount(), imageCount);
    QCOMPARE(header.mipmapCount(), mipmapCount);
    QCOMPARE(header.loopCount(), loopCount);

    for (int level = 0, i = 0; level < mipmapCount; ++level) {
        for (int index = 0; index < imageCount; ++index, i++) {
            QCOMPARE(contents.image(index, level), data.images[i]);
        }
    }
}

void TestImageIO::write_data()
{
    QTest::addColumn<int>("type");
    QTest::addColumn<QSize>("size");
    QTest::addColumn<int>("imageFormat");
    QTest::addColumn<QString>("name");
    QTest::addColumn<int>("imageCount");
    QTest::addColumn<int>("mipmapCount");
    QTest::addColumn<int>("loopCount");

    QTest::newRow("1x1, AGRB32, 1x1, -1") << int(ImageResource::Type::Image) << QSize(1, 1) << int(QImage::Format_ARGB32) << QString() << 1 << 1 << -1;
    QTest::newRow("64x64, AGRB32, 1x1, -1") << int(ImageResource::Type::Image) << QSize(1, 1) << int(QImage::Format_ARGB32) << QString() << 1 << 1 << -1;
    QTest::newRow("64x64, GRB32, 1x1, -1") << int(ImageResource::Type::Image) << QSize(1, 1) << int(QImage::Format_RGB32) << QString() << 1 << 1 << -1;
//    QTest::newRow("64x64, invalid, 1x1, -1") << int(ImageResource::Type::Image) << QSize(1, 1) << int(QImage::Format_Invalid) << QString() << 1 << 1 << -1;
    QTest::newRow("image1x1") << int(ImageResource::Type::Image) << QSize(1, 1) << int(QImage::Format_ARGB32) << QString() << 1 << 1 << -1;
    QTest::newRow("image1x2") << int(ImageResource::Type::Image) << QSize(2, 2) << int(QImage::Format_ARGB32) << QString() << 1 << 2 << -1;
    QTest::newRow("image1x4") << int(ImageResource::Type::Image) << QSize(8, 8) << int(QImage::Format_ARGB32) << QString() << 1 << 4 << -1;
    QTest::newRow("image2x1") << int(ImageResource::Type::Image) << QSize(1, 1) << int(QImage::Format_ARGB32) << QString() << 2 << 1 << -1;
    QTest::newRow("image4x1") << int(ImageResource::Type::Image) << QSize(1, 1) << int(QImage::Format_ARGB32) << QString() << 4 << 1 << -1;
    QTest::newRow("image4x4") << int(ImageResource::Type::Image) << QSize(8, 8) << int(QImage::Format_ARGB32) << QString() << 4 << 4 << -1;
    QTest::newRow("named image1x1") << int(ImageResource::Type::Image) << QSize(1, 1) << int(QImage::Format_ARGB32) << QString("name") << 1 << 1 << -1;
    QTest::newRow("image1x1 loop count = 1") << int(ImageResource::Type::Image) << QSize(1, 1) << int(QImage::Format_ARGB32) << QString() << 1 << 1 << 1;
}

void TestImageIO::write()
{
    QFETCH(int, type);
    QFETCH(QSize, size);
    QFETCH(int, imageFormat);
    QFETCH(QString, name);
    QFETCH(int, imageCount);
    QFETCH(int, mipmapCount);
    QFETCH(int, loopCount);

    QBuffer buffer;

    ImageIO io;
    io.setDevice(&buffer);
    io.setMimeType("application/octet-stream");

    ImageHeader header;
    header.setType(ImageResource::Type(type));
    header.setSize(size);
    header.setImageFormat(QImage::Format(imageFormat));
    header.setName(name);
    header.setImageCount(imageCount);
    header.setHasMipmaps(mipmapCount > 1);
    QCOMPARE(header.mipmapCount(), mipmapCount);
    header.setLoopCount(loopCount);
    ImageContents contents(header);
    QVERIFY(!contents.isNull());

    auto images = generateImages(size, imageCount, mipmapCount, QImage::Format(imageFormat));

    for (int level = 0, i = 0; level < mipmapCount; ++level) {
        for (int index = 0; index < imageCount; ++index, i++) {
            contents.setImage(images[i], index, level);
            QCOMPARE(contents.image(index, level), images[i]);
        }
    }

    const auto ok = io.write(contents);
    QVERIFY2(ok, io.error().errorString().toUtf8().constData());

    TestImageData data;
    buffer.close();

    QVERIFY(buffer.open(QIODevice::ReadOnly));
    QDataStream stream(&buffer);
    stream >> data;

    QCOMPARE(data.type, ImageResource::Type(type));
    QCOMPARE(data.size, size);
    if (imageFormat != QImage::Format_Invalid)
        QCOMPARE(data.imageFormat, QImage::Format(imageFormat));
    else
        QCOMPARE(data.imageFormat, QImage::Format_ARGB32); // fallback in generateImage
    QCOMPARE(data.name, name);
    QCOMPARE(data.imageCount, imageCount);
    QCOMPARE(data.hasMipmaps, mipmapCount > 1);
    QCOMPARE(data.loopCount, loopCount);

    for (int level = 0, i = 0; level < mipmapCount; ++level) {
        for (int index = 0; index < imageCount; ++index, i++) {
            QCOMPARE(data.images[i], images[i]);
        }
    }
}

void TestImageIO::writeOptions_data()
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

void TestImageIO::writeOptions()
{
    QFETCH(int, compression);
    QFETCH(int, gamma);
    QFETCH(int, quality);

    const int imageCount = 1;
    const int mipmapCount = 1;
    const QSize size(64, 64);
    const QImage::Format imageFormat(QImage::Format_ARGB32);

    ImageHeader header;
    header.setType(ImageResource::Type::Image);
    header.setSize(size);
    ImageContents contents;

    auto images = generateImages(size, imageCount, mipmapCount, QImage::Format(imageFormat));

    for (int level = 0, i = 0; level < mipmapCount; ++level) {
        for (int index = 0; index < imageCount; ++index, i++) {
            contents.setImage(images[i], index, level);
        }
    }

    QBuffer buffer;
    QVERIFY(buffer.open(QIODevice::ReadWrite));

    ImageIO io;
    io.setDevice(&buffer);
    io.setMimeType("application/octet-stream");

    ImageOptions options;
    if (compression != -1)
        options.setCompression(compression);
    if (gamma != -1)
        options.setGamma(gamma);
    if (quality != -1)
        options.setQuality(quality);

    const auto ok = io.write(contents, options);
    QVERIFY2(ok, io.error().errorString().toUtf8().constData());

    TestImageData data;
    buffer.seek(0);
    QDataStream stream(&buffer);
    stream >> data;

    if (compression != -1)
        QCOMPARE(data.compression, compression);
    if (gamma != -1)
        QCOMPARE(data.gamma, gamma);
    if (quality != -1)
        QCOMPARE(data.quality, quality);
}

QTEST_APPLESS_MAIN(TestImageIO)

#include "tst_imageio.moc"

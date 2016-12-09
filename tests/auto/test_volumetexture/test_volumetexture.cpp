#include <QtTest>
#include <VolumeTexture>

Q_DECLARE_METATYPE(QImage::Format)

class TestVolumeTexture : public QObject
{
    Q_OBJECT
private slots:
    void defaultValues();
    void construct1_data();
    void construct1();
    void construct2_data();
    void construct2();
    void pixel();
    void slice();
};

void TestVolumeTexture::defaultValues()
{
    VolumeTexture tex;
    QCOMPARE(tex.isNull(), true);
    QCOMPARE(tex.width(), 0);
    QCOMPARE(tex.height(), 0);
    QCOMPARE(tex.depth(), 0);
    QCOMPARE(tex.format(), QImage::Format_Invalid);
}

void TestVolumeTexture::construct1_data()
{
    QTest::addColumn<int>("width");
    QTest::addColumn<int>("height");
    QTest::addColumn<int>("depth");
    QTest::addColumn<QImage::Format>("imageFormat");
    QTest::addColumn<bool>("valid");

    QTest::newRow("1x1x1 ARGB") << 1 << 1 << 1 << QImage::Format_ARGB32 << true;
    QTest::newRow("64x64x64 ARGB") << 64 << 64 << 64 << QImage::Format_ARGB32 << true;
    QTest::newRow("0x1x1 ARGB") << 0 << 1 << 1 << QImage::Format_ARGB32 << false;
    QTest::newRow("-1x1x1 ARGB") << -1 << 1 << 1 << QImage::Format_ARGB32 << false;
    QTest::newRow("1x0x1 ARGB") << 1 << 0 << 1 << QImage::Format_ARGB32 << false;
    QTest::newRow("1x-1x1 ARGB") << 1 << -1 << 1 << QImage::Format_ARGB32 << false;
    QTest::newRow("1x1x0 ARGB") << 1 << 1 << 0 << QImage::Format_ARGB32 << false;
    QTest::newRow("1x1x-1 ARGB") << 1 << 1 << -1 << QImage::Format_ARGB32 << false;
    QTest::newRow("1x1x1 Invalid") << 1 << 1 << -1 << QImage::Format_Invalid << false;
}

void TestVolumeTexture::construct1()
{
    QFETCH(int, width);
    QFETCH(int, height);
    QFETCH(int, depth);
    QFETCH(QImage::Format, imageFormat);
    QFETCH(bool, valid);

    VolumeTexture tex(width, height, depth, imageFormat);
    QCOMPARE(!tex.isNull(), valid);
    if (!valid)
        return;
    QCOMPARE(tex.width(), width);
    QCOMPARE(tex.height(), height);
    QCOMPARE(tex.depth(), depth);
    QCOMPARE(tex.format(), imageFormat);
}

void TestVolumeTexture::construct2_data()
{
    QTest::addColumn<QVector<QImage>>("slices");
    QTest::addColumn<bool>("valid");

    QImage red(64, 64, QImage::Format_ARGB32);
    red.fill(Qt::red);
    QImage blue(64, 64, QImage::Format_ARGB32);
    blue.fill(Qt::blue);
    QImage invalid;
    QImage small(32, 32, QImage::Format_ARGB32);
    small.fill(Qt::white);

    QTest::newRow("invalid") << QVector<QImage>{invalid} << false;
    QTest::newRow("red") << QVector<QImage>{red} << true;
    QTest::newRow("red, blue") << QVector<QImage>{red, blue} << true;
    QTest::newRow("red, blue, invalid") << QVector<QImage>{red, blue, invalid} << false;
    QTest::newRow("red, blue, small") << QVector<QImage>{red, blue, small} << false;
}

void TestVolumeTexture::construct2()
{
    QFETCH(QVector<QImage>, slices);
    QFETCH(bool, valid);

    VolumeTexture tex(slices);
    QCOMPARE(!tex.isNull(), valid);

    if (!valid)
        return;

    for (int z = 0; z < slices.size(); ++z) {
        QCOMPARE(tex.slice(z), slices.at(z));
    }
}

void TestVolumeTexture::pixel()
{
    int width = 64;
    int height = 64;
    int depth = 64;
    VolumeTexture tex(width, height, depth, QImage::Format_ARGB32);

    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            for (int z = 0; z < depth; ++z) {
                auto value = qRgb(255 - x, 255 - y, 255 - z);
                tex.setPixel(x, y, z, value);
                QCOMPARE(tex.pixel(x, y, z), value);
            }
        }
    }
}

void TestVolumeTexture::slice()
{
    int width = 64;
    int height = 64;
    int depth = 2;
    QImage::Format format = QImage::Format_ARGB32;

    QImage red(width, height, format);
    red.fill(Qt::red);
    QImage blue(width, height, format);
    blue.fill(Qt::blue);

    VolumeTexture tex(width, height, depth, format);
    tex.setSlice(0, red);
    tex.setSlice(1, blue);
    QCOMPARE(tex.slice(0), red);
    QCOMPARE(tex.slice(1), blue);
}

QTEST_MAIN(TestVolumeTexture)

#include "test_volumetexture.moc"

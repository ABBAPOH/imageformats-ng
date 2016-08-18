#include <QtTest>

#include <ImageContents>
#include <ImageExifMeta>

class TestImageContents : public QObject
{
    Q_OBJECT
private slots:
    void defaultValues();
    void setters();
    void equals_data();
    void equals();
};

void TestImageContents::defaultValues()
{
    ImageContents contents;
    QCOMPARE(contents.type(), ImageContents::Invalid);
    QCOMPARE(contents.size(), QSize());
    QCOMPARE(contents.imageFormat(), QImage::Format_Invalid);
    QCOMPARE(contents.name(), QString());
    QCOMPARE(contents.imageCount(), 1);
    QCOMPARE(contents.mipmapCount(), 0);
    QCOMPARE(contents.loopCount(), -1);
    QCOMPARE(contents.exifMeta(), ImageExifMeta());
    QCOMPARE(contents.image(), QImage());
}

void TestImageContents::setters()
{
    ImageContents contents;

    contents.setType(ImageContents::Animation);
    QCOMPARE(contents.type(), ImageContents::Animation);

    contents.setSize(QSize(64, 64));
    QCOMPARE(contents.size(), QSize(64, 64));

    contents.setImageFormat(QImage::Format_ARGB32);
    QCOMPARE(contents.imageFormat(), QImage::Format_ARGB32);

    contents.setName(QString("name"));
    QCOMPARE(contents.name(), QString("name"));

    contents.setImageCount(2);
    QCOMPARE(contents.imageCount(), 2);

    contents.setMipmapCount(2);
    QCOMPARE(contents.mipmapCount(), 2);

    contents.setLoopCount(10);
    QCOMPARE(contents.loopCount(), 10);

    for (int i = 0; i < contents.imageCount(); ++i) {
        for (int j = 0; j < contents.mipmapCount(); ++j) {
            int size = 1 << (contents.mipmapCount() - j - 1);
            QImage image(size, size, QImage::Format_ARGB32);
            image.fill(QColor(255 * i / 2, 255 * j / 2, 255 * (i + j) / 4));
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

    ImageContents c1;
    ImageContents c2;

    QVERIFY(c1 == c2);

    c1.setSize(size);
    c2.setSize(size);

    c1.setName(name);
    c2.setName(name);

    c1.setImageCount(imageCount);
    c2.setImageCount(imageCount);

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

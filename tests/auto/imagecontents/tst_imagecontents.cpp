#include <QtTest>

#include <ImageContents>
#include <ImageExifMeta>

class TestImageContents : public QObject
{
    Q_OBJECT
private slots:
    void defaultValues();
    void setters();
    void equals();
};

void TestImageContents::defaultValues()
{
    ImageContents contents;
    QCOMPARE(contents.type(), ImageContents::Image);
    QCOMPARE(contents.size(), QSize());
    QCOMPARE(contents.imageFormat(), QImage::Format_Invalid);
    QCOMPARE(contents.name(), QString());
    QCOMPARE(contents.imageCount(), 1);
    QCOMPARE(contents.mipmapCount(), 1);
    QCOMPARE(contents.loopCount(), -1);
    QCOMPARE(contents.exifMeta(), ImageExifMeta());
    QCOMPARE(contents.image(), QImage());
}

void TestImageContents::setters()
{
    ImageContents c;

    c.setType(ImageContents::Animation);
    QCOMPARE(c.type(), ImageContents::Animation);

    c.setSize(QSize(64, 64));
    QCOMPARE(c.size(), QSize(64, 64));

    c.setImageFormat(QImage::Format_ARGB32);
    QCOMPARE(c.imageFormat(), QImage::Format_ARGB32);

    c.setName(QString("name"));
    QCOMPARE(c.name(), QString("name"));

    c.setImageCount(2);
    QCOMPARE(c.imageCount(), 2);

    c.setMipmapCount(2);
    QCOMPARE(c.mipmapCount(), 2);

    c.setLoopCount(10);
    QCOMPARE(c.loopCount(), 10);

    for (int i = 0; i < c.imageCount(); ++i) {
        for (int j = 0; j < c.mipmapCount(); ++j) {
            int size = 1 << (c.mipmapCount() - j - 1);
            QImage image(size, size, QImage::Format_ARGB32);
            image.fill(QColor(255 * i / 2, 255 * j / 2, 255 * (i + j) / 4));
            c.setImage(image, i, j);
            QCOMPARE(c.image(i, j), image);
        }
    }
}

void TestImageContents::equals()
{
    ImageContents c1;
    ImageContents c2;

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

QTEST_APPLESS_MAIN(TestImageContents)

#include "tst_imagecontents.moc"

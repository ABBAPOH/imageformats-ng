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
    QCOMPARE(contents.exifMeta(), ImageExifMeta());
    QCOMPARE(contents.image(), QImage());
}

void TestImageContents::setters()
{
    ImageHeader header;

    header.setType(ImageHeader::Animation);
    QCOMPARE(header.type(), ImageHeader::Animation);

    header.setSize(QSize(64, 64));
    QCOMPARE(header.size(), QSize(64, 64));

    header.setImageFormat(QImage::Format_ARGB32);
    QCOMPARE(header.imageFormat(), QImage::Format_ARGB32);

    header.setName(QString("name"));
    QCOMPARE(header.name(), QString("name"));

    header.setImageCount(2);
    QCOMPARE(header.imageCount(), 2);

    header.setMipmapCount(2);
    QCOMPARE(header.mipmapCount(), 2);

    header.setLoopCount(10);
    QCOMPARE(header.loopCount(), 10);

    ImageContents contents;
    contents.setHeader(header);

    for (int i = 0; i < header.imageCount(); ++i) {
        for (int j = 0; j < header.mipmapCount(); ++j) {
            int size = 1 << (header.mipmapCount() - j - 1);
            QImage image(size, size, QImage::Format_ARGB32);
            image.fill(QColor(255 * i / 2, 255 * j / 2, 255 * (i + j) / 4));
            contents.setImage(image, i, j);
            QCOMPARE(contents.image(i, j), image);
        }
    }
}

void TestImageContents::equals()
{
    ImageHeader header;
    ImageContents c1;
    ImageContents c2;

    QVERIFY(c1 == c2);

    header.setSize(QSize(64, 64));
    header.setName(QString("name"));
    header.setImageCount(2);

    for (int i = 0; i < header.imageCount(); ++i) {
        QImage image(header.size(), QImage::Format_ARGB32);
        image.fill(QColor(255 * i / 2, 255 * i / 2, 255 * (2*i) / 4));
        c1.setImage(image, i);
    }

    QVERIFY(c1 != c2);

    for (int i = 0; i < header.imageCount(); ++i) {
        QImage image(header.size(), QImage::Format_ARGB32);
        image.fill(QColor(255 * i / 2, 255 * i / 2, 255 * (2*i) / 4));
        c2.setImage(image, i);
    }

    QVERIFY(c1 == c2);
}

QTEST_APPLESS_MAIN(TestImageContents)

#include "tst_imagecontents.moc"

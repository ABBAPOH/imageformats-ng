#include <QtTest>

#include <imagecontents.h>

class TestImageContents : public QObject
{
    Q_OBJECT
private slots:
    void defaultValues();
    void setters();
};

void TestImageContents::defaultValues()
{
    ImageContents contents;
    QCOMPARE(contents.type(), ImageContents::Image);
    QCOMPARE(contents.size(), QSize());
    QCOMPARE(contents.imageFormat(), QImage::Format_Invalid);
    QCOMPARE(contents.imageCount(), 1);
    QCOMPARE(contents.mipmapCount(), 1);
    QCOMPARE(contents.loopCount(), -1);
    QCOMPARE(contents.exifMeta(), ImageExifMeta());
    QCOMPARE(contents.image(), QImage());
}

void TestImageContents::setters()
{
    ImageContents doc;

    doc.setType(ImageContents::Animation);
    QCOMPARE(doc.type(), ImageContents::Animation);

    doc.setSize(QSize(64, 64));
    QCOMPARE(doc.size(), QSize(64, 64));

    doc.setImageFormat(QImage::Format_ARGB32);
    QCOMPARE(doc.imageFormat(), QImage::Format_ARGB32);

    doc.setImageCount(2);
    QCOMPARE(doc.imageCount(), 2);

    doc.setMipmapCount(2);
    QCOMPARE(doc.mipmapCount(), 2);

    doc.setLoopCount(10);
    QCOMPARE(doc.loopCount(), 10);

    for (int i = 0; i < doc.imageCount(); ++i) {
        for (int j = 0; j < doc.mipmapCount(); ++j) {
            int size = 1 << (doc.mipmapCount() - j - 1);
            QImage image(size, size, QImage::Format_ARGB32);
            image.fill(QColor(255 * i / 2, 255 * j / 2, 255 * (i + j) / 4));
            doc.setImage(image, i, j);
            QCOMPARE(doc.image(i, j), image);
        }
    }
}

QTEST_APPLESS_MAIN(TestImageContents)

#include "tst_imagecontents.moc"

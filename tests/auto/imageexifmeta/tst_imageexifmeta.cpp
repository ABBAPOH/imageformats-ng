#include <QtTest>

#include <ImageExifMeta>

class TestImageExifMeta : public QObject
{
    Q_OBJECT
private slots:
    void defaultValues();
    void setters();
};

void TestImageExifMeta::defaultValues()
{
    ImageExifMeta meta;
    QVERIFY(meta.isEmpty());
}

void TestImageExifMeta::setters()
{
    ImageExifMeta meta;

    meta.setValue(ImageExifMeta::TagImageWidth, 640);
    meta.setValue(ImageExifMeta::TagImageHeight, 480);
    meta.setValue(ImageExifMeta::TagOrientation, QVariant::fromValue<ImageExifMeta::Orientation>(ImageExifMeta::OrientationHorizontal));

    QCOMPARE(meta.value(ImageExifMeta::TagImageWidth).toInt(), 640);
    QCOMPARE(meta.value(ImageExifMeta::TagImageHeight).toInt(), 480);
    QCOMPARE(meta.value(ImageExifMeta::TagOrientation).value<ImageExifMeta::Orientation>(),
             ImageExifMeta::OrientationHorizontal);
}

QTEST_APPLESS_MAIN(TestImageExifMeta)

#include "tst_imageexifmeta.moc"

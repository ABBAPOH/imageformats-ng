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
    meta.setValue(ImageExifMeta::TagDocumentName, QStringLiteral("name"));
    meta.setValue(ImageExifMeta::TagOrientation, QVariant::fromValue(ImageExifMeta::OrientationHorizontal));

    QCOMPARE(meta.value(ImageExifMeta::TagImageWidth).toInt(), 640);
    QCOMPARE(meta.value(ImageExifMeta::TagImageHeight).toInt(), 480);
    QCOMPARE(meta.value(ImageExifMeta::TagDocumentName).toString(), QStringLiteral("name"));
    QCOMPARE(meta.value(ImageExifMeta::TagOrientation).value<ImageExifMeta::Orientation>(),
             ImageExifMeta::OrientationHorizontal);

    meta.setImageWidth(800);
    meta.setImageHeight(600);
    meta.setDocumentName(QStringLiteral("new name"));
    meta.setOrientation(ImageExifMeta::OrientationHMirror);

    QCOMPARE(*meta.imageWidth(), 800);
    QCOMPARE(*meta.imageHeight(), 600);
    QCOMPARE(*meta.documentName(), QStringLiteral("new name"));
    QCOMPARE(*meta.orientation(), ImageExifMeta::OrientationHMirror);
}

QTEST_APPLESS_MAIN(TestImageExifMeta)

#include "tst_imageexifmeta.moc"

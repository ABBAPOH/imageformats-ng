#include <QtTest>

#include <imagedocument.h>

class TestImageDocument : public QObject
{
    Q_OBJECT
private slots:
    void defaultValues();
    void setters();
};

void TestImageDocument::defaultValues()
{
    ImageDocument doc;
    QCOMPARE(doc.mipmapCount(), 1);
    QCOMPARE(doc.frameCount(), 1);
    QCOMPARE(doc.sides(), ImageIndex::NoSides);
    QCOMPARE(doc.sliceCount(), 1);
}

void TestImageDocument::setters()
{
    ImageDocument doc;

    doc.setMipmapCount(2);
    QCOMPARE(doc.mipmapCount(), 2);

    doc.setFrameCount(2);
    QCOMPARE(doc.frameCount(), 2);

    doc.setSides(ImageIndex::Sides(ImageIndex::PositiveX | ImageIndex::NegaviveX));
    QCOMPARE(doc.sides(), ImageIndex::Sides(ImageIndex::PositiveX | ImageIndex::NegaviveX));

    doc.setSliceCount(2);
    QCOMPARE(doc.sliceCount(), 2);
}

QTEST_APPLESS_MAIN(TestImageDocument)

#include "tst_imagedocument.moc"

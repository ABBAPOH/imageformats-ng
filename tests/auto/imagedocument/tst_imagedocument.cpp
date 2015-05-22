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
    QCOMPARE(doc.resourceCount(), 1);
    QCOMPARE(doc.sides(), ImageResource::NoSides);
//    QCOMPARE(doc.sliceCount(), 1);
}

void TestImageDocument::setters()
{
    ImageDocument doc;

    doc.setMipmapCount(2);
    QCOMPARE(doc.mipmapCount(), 2);

    doc.setResourceCount(2);
    QCOMPARE(doc.resourceCount(), 2);

    doc.setSides(ImageResource::Sides(ImageResource::PositiveX | ImageResource::NegaviveX));
    QCOMPARE(doc.sides(), ImageResource::Sides(ImageResource::PositiveX | ImageResource::NegaviveX));

//    doc.setSliceCount(2);
//    QCOMPARE(doc.sliceCount(), 2);
}

QTEST_APPLESS_MAIN(TestImageDocument)

#include "tst_imagedocument.moc"

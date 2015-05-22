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
    QCOMPARE(doc.resourceCount(), 1);
//    QCOMPARE(doc.sliceCount(), 1);
}

void TestImageDocument::setters()
{
    ImageDocument doc;

    doc.setResourceCount(2);
    QCOMPARE(doc.resourceCount(), 2);

//    doc.setSliceCount(2);
//    QCOMPARE(doc.sliceCount(), 2);
}

QTEST_APPLESS_MAIN(TestImageDocument)

#include "tst_imagedocument.moc"

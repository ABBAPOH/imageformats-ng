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
    QCOMPARE(doc.imageCount(), 0);
//    QCOMPARE(doc.sliceCount(), 1);
}

void TestImageDocument::setters()
{
    ImageDocument doc;

//    doc.setSliceCount(2);
//    QCOMPARE(doc.sliceCount(), 2);
}

QTEST_APPLESS_MAIN(TestImageDocument)

#include "tst_imagedocument.moc"

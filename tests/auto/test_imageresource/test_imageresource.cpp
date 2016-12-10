#include <QtTest>
#include <ImageResource>

class TestImageResource : public QObject
{
    Q_OBJECT
private slots:
    void defaultConstructed();
    void image();
};

void TestImageResource::defaultConstructed()
{
    ImageResource res;
    QVERIFY(res.isNull());
    QCOMPARE(res.type(), ImageResource::Type::Invalid);
    QVERIFY(res.image().isNull());
    QVERIFY(res.cubeTexture().isNull());
    QVERIFY(res.volumeTexture().isNull());
}

void TestImageResource::image()
{
    QImage image(64, 64, QImage::Format_ARGB32);
    image.fill(Qt::white);

    ImageResource res(image);
    QVERIFY(!res.isNull());
    QCOMPARE(res.type(), ImageResource::Type::Image);
    QCOMPARE(res.image(), image);
    QVERIFY(res.cubeTexture().isNull());
    QVERIFY(res.volumeTexture().isNull());
}

QTEST_MAIN(TestImageResource)

#include "test_imageresource.moc"

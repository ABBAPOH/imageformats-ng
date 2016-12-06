#include <QtTest>
#include <VolumeTexture>

class TestVolumeTexture : public QObject
{
    Q_OBJECT
private slots:
    void defaultValues();
};

void TestVolumeTexture::defaultValues()
{
    VolumeTexture tex;
    QCOMPARE(tex.isNull(), true);
    QCOMPARE(tex.width(), -1);
    QCOMPARE(tex.height(), -1);
    QCOMPARE(tex.depth(), -1);
    QCOMPARE(tex.format(), QImage::Format_Invalid);
}

QTEST_MAIN(TestVolumeTexture)

#include "test_volumetexture.moc"

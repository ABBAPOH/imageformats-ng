#include <QtTest>

#include <ImageIO>
#include <QMimeDatabase>

class TestJpeg : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void readImage_data();
    void readImage();
    void testHeader_data();
//    void testHeader();
};

void TestJpeg::initTestCase()
{
    qApp->addLibraryPath(qApp->applicationDirPath() + ImageIO::pluginsDirPath());
}

void TestJpeg::readImage_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<bool>("success");

    QTest::newRow("beavis") << QString(":/beavis.jpg") << true;
    QTest::newRow("qtbug13653") << QString(":/qtbug13653-no_eoi.jpg") << true;
    QTest::newRow("corrupt") << QString(":/corrupt.jpg") << false;
}

void TestJpeg::readImage()
{
    QFETCH(QString, fileName);
    QFETCH(bool, success);

    ImageIO io(fileName);
    io.read();
    auto ok = io.error();
    QVERIFY2(ok == ImageIO::Error::NoError, ok.errorString().toUtf8().constData());
}

void TestJpeg::testHeader_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QSize>("size");
    QTest::addColumn<int>("format");

    QTest::newRow("beavis") << QString(":/beavis.jpg") << QSize(350, 350) << int(QImage::Format_Indexed8);
    QTest::newRow("qtbug13653") << QString(":/qtbug13653-no_eoi.jpg") << QSize(240, 180) << int(QImage::Format_RGB32);
}

//void TestJpeg::testHeader()
//{
//    QFETCH(QString, fileName);
//    QFETCH(QSize, size);
//    QFETCH(int, format);

//    ImageDocument doc(fileName);
//    auto ok = doc.openHeader();
//    QVERIFY2(ok, ok.errorString().toUtf8().constData());

//    QCOMPARE(doc.contents().size(), size);
//    QCOMPARE(int(doc.contents().imageFormat()), format);
//    QVERIFY(doc.contents().image().isNull());
//}

QTEST_MAIN(TestJpeg)

#include "tst_jpeg.moc"

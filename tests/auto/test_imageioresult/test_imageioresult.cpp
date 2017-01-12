#include <QtTest>

#include <ImageIOResult>

Q_DECLARE_METATYPE(ImageIOResult::Status)

class TestImageIOResult : public QObject
{
    Q_OBJECT
private slots:
    void defaultConstruction();
    void construction_data();
    void construction();
    void copy();
    void assign();
};

void TestImageIOResult::defaultConstruction()
{
    ImageIOResult result;
    QCOMPARE(result.status(), ImageIOResult::Status::Ok);
    QVERIFY(result.toBool());
    QVERIFY(bool(result));
    QCOMPARE(!result, false);
}

void TestImageIOResult::construction_data()
{
    QTest::addColumn<ImageIOResult::Status>("status");
    QTest::addColumn<bool>("ok");

    QTest::newRow("Ok") << ImageIOResult::Status::Ok << true;
    QTest::newRow("InvalidMimeType") << ImageIOResult::Status::InvalidMimeType << false;
    QTest::newRow("FileNotFound") << ImageIOResult::Status::FileNotFound << false;
    QTest::newRow("DeviceError") << ImageIOResult::Status::DeviceError << false;
    QTest::newRow("UnsupportedMimeType") << ImageIOResult::Status::UnsupportedMimeType << false;
    QTest::newRow("IOError") << ImageIOResult::Status::HandlerError << false;
}

void TestImageIOResult::construction()
{
    QFETCH(ImageIOResult::Status, status);
    QFETCH(bool, ok);

    ImageIOResult result(status);
    QCOMPARE(result.status(), status);
    QCOMPARE(result.toBool(), ok);
    QCOMPARE(bool(result), ok);
    QCOMPARE(!result, !ok);
}

void TestImageIOResult::copy()
{
    ImageIOResult r1(ImageIOResult::Status::Ok);
    ImageIOResult r2(r1);

    QCOMPARE(r1.status(), r2.status());
    QCOMPARE(r1.toBool(), r2.toBool());
    QCOMPARE(bool(r1), bool(r2));
    QCOMPARE(!r1, !r2);
    QVERIFY(r1 == r2);
    QVERIFY(!(r1 != r2));
}

void TestImageIOResult::assign()
{
    ImageIOResult r1(ImageIOResult::Status::Ok);
    ImageIOResult r2(ImageIOResult::Status::InvalidMimeType);

    QVERIFY(!(r1 == r2));
    QVERIFY(r1 != r2);

    r1 = r2;

    QCOMPARE(r1.status(), r2.status());
    QCOMPARE(r1.toBool(), r2.toBool());
    QCOMPARE(bool(r1), bool(r2));
    QCOMPARE(!r1, !r2);
    QVERIFY((r1 == r2));
    QVERIFY(!(r1 != r2));
}

QTEST_APPLESS_MAIN(TestImageIOResult)

#include "test_imageioresult.moc"

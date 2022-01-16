#include "asciiarrows_tests.h"

#include <QtTest>
#include <QVector>
#include <QPair>

#include "asciiarrows.h"

using Arrow = std::tuple<QPoint, int, Qt::Orientation>;
using Arrows = QVector<Arrow>;

Q_DECLARE_METATYPE(Arrow);

void AsciiArrows::Tests::arrows_data()
{
    QTest::addColumn<Arrows>("arrows");
    QTest::addColumn<QSize>("size");
    QTest::addColumn<QString>("expected");
    const char *zero2x2 =
            "..\n"
            "..\n";
    const char *zero3x2 =
            "...\n"
            "...\n";
    const char *zero2x3 =
            "..\n"
            "..\n"
            "..\n";
    QTest::addRow("zero 2x2") << Arrows{} << QSize(2, 2) << QString(zero2x2);
    QTest::addRow("zero 3x2") << Arrows{} << QSize(3, 2) << QString(zero3x2);
    QTest::addRow("zero 2x3") << Arrows{} << QSize(2, 3) << QString(zero2x3);

    const char *oneVertIn3x3 =
            "...\n"
            ".↑.\n"
            "...\n";
    const char *fourVertIn3x3 =
            ".↓.\n"
            ".↑↓\n"
            "↑..\n";
    const char *allVertIn1x2=
            "│\n"
            "│\n";
    const char *oneVertIn1x2=
            ".\n"
            "↑\n";
    const char *oneVertFullIn1x2=
            "↑\n"
            "│\n";
    const char *oneVertFullIn1x4=
            "↑\n"
            "│\n"
            "│\n"
            "│\n";
    QTest::addRow("one vert") << Arrows{Arrow({1, 1}, 1, Qt::Vertical)} << QSize(3, 3) << QString(oneVertIn3x3);
    QTest::addRow("three vert") << Arrows{Arrow({1, 1}, 1, Qt::Vertical), Arrow({0, 0}, 1, Qt::Vertical), Arrow({2, 1}, -1, Qt::Vertical), Arrow({1, 2}, -1, Qt::Vertical)} << QSize(3, 3) << QString(fourVertIn3x3);
    QTest::addRow("one vert from outside") << Arrows{Arrow({0, -5}, 6, Qt::Vertical)} << QSize(1, 2) << QString(oneVertIn1x2);
    QTest::addRow("one vert outside") << Arrows{Arrow({1, -5}, 5, Qt::Vertical), Arrow({0, 6}, -4, Qt::Vertical)} << QSize(2, 2) << QString(zero2x2);
    QTest::addRow("one vert pass throught") << Arrows{Arrow({0, -10}, 20, Qt::Vertical)} << QSize(1, 2) << QString(allVertIn1x2);
    QTest::addRow("two vert on each other") << Arrows{Arrow({0, 0}, 2, Qt::Vertical), Arrow({0, 0}, 1, Qt::Vertical)} << QSize(1, 2) << QString(oneVertFullIn1x2);
    QTest::addRow("two vert overlap") << Arrows{Arrow({0, 1}, 2, Qt::Vertical), Arrow({0, 0}, 4, Qt::Vertical)} << QSize(1, 4) << QString(oneVertFullIn1x4);
    QTest::addRow("three vert seq") << Arrows{Arrow({0, 0}, 2, Qt::Vertical), Arrow({0, 1}, 2, Qt::Vertical), Arrow({0, 2}, 2, Qt::Vertical)} << QSize(1, 4) << QString(oneVertFullIn1x4);
    QTest::addRow("two vert close ") << Arrows{Arrow({0, 0}, 2, Qt::Vertical), Arrow({0, 1}, -2, Qt::Vertical)} << QSize(1, 2) << QString(allVertIn1x2);

    const char *oneHoriIn3x3 =
            "...\n"
            ".→.\n"
            "...\n";
    const char *fourHoriIn3x3 =
            "..→\n"
            ".→←\n"
            "←..\n";
    const char *oneHoriIn2x1 =
            "→.\n";
    const char *allHoriIn2x1 =
            "──\n";
    const char *oneHoriFullIn2x1 =
            "─→\n";
    const char *oneHoriFullIn4x1 =
            "───→\n";
    QTest::addRow("one hori") << Arrows{Arrow({1, 1}, 1, Qt::Horizontal)} << QSize(3, 3) << QString(oneHoriIn3x3);
    QTest::addRow("three hori") << Arrows{Arrow({1, 1}, 1, Qt::Horizontal), Arrow({0, 0}, -1, Qt::Horizontal), Arrow({2, 1}, -1, Qt::Horizontal), Arrow({2, 2}, 1, Qt::Horizontal)} << QSize(3, 3) << QString(fourHoriIn3x3);
    QTest::addRow("one hori from outside") << Arrows{Arrow({-5, 0}, 6, Qt::Horizontal)} << QSize(2, 1) << QString(oneHoriIn2x1);
    QTest::addRow("one hori outside") << Arrows{Arrow({-5, 1}, 5, Qt::Horizontal), Arrow({6, 0}, -4, Qt::Horizontal)} << QSize(2, 2) << QString(zero2x2);
    QTest::addRow("one hori pass throught") << Arrows{Arrow({-10, 0}, 20, Qt::Horizontal)} << QSize(2, 1) << QString(allHoriIn2x1);
    QTest::addRow("two hori on each other") << Arrows{Arrow({0, 0}, 2, Qt::Horizontal), Arrow({0, 0}, 1, Qt::Horizontal)} << QSize(2, 1) << QString(oneHoriFullIn2x1);
    QTest::addRow("two hori overlap") << Arrows{Arrow({1, 0}, 2, Qt::Horizontal), Arrow({0, 0}, 4, Qt::Horizontal)} << QSize(4, 1) << QString(oneHoriFullIn4x1);
    QTest::addRow("three hori seq") << Arrows{Arrow({0, 0}, 2, Qt::Horizontal), Arrow({1, 0}, 2, Qt::Horizontal), Arrow({2, 0}, 2, Qt::Horizontal)} << QSize(4, 1) << QString(oneHoriFullIn4x1);
    QTest::addRow("two hori close ") << Arrows{Arrow({0, 0}, 2, Qt::Horizontal), Arrow({1, 0}, -2, Qt::Horizontal)} << QSize(2, 1) << QString(allHoriIn2x1);


    const char *oneFromOther =
            "↑..\n"
            "├─→\n"
            "│..\n";
    QTest::addRow("start at center") << Arrows{Arrow({0, 0}, 3, Qt::Vertical), Arrow({0, 1}, 3, Qt::Horizontal)} << QSize(3, 3) << QString(oneFromOther);
}

void AsciiArrows::Tests::arrows()
{
    QFETCH(Arrows, arrows);
    QFETCH(QSize, size);
    QFETCH(QString, expected);

    const QRect rect(QPoint(0, 0), size);
    int b[size.width() * size.height()];
    AsciiArrows::clearBuffer(b, size);
    for (const Arrow &arrow : arrows)
        AsciiArrows::drawArrow(b, rect, std::get<0>(arrow), std::get<1>(arrow), std::get<2>(arrow));
    const QString res = AsciiArrows::bufferToString(b, size);
    if (res != expected)
        qDebug().noquote() << ("\nres:\n" + res) << ("expected:\n" + expected);
    QCOMPARE(res, expected);
}

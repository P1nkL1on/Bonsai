#include "asciiarrows_tests.h"

#include <QtTest>
#include <QVector>
#include <QPair>

#include "asciiarrows.h"

using Arrow = std::tuple<QPoint, int, Qt::Orientation>;
using Arrows = QVector<Arrow>;

// for std::next_permutation
bool operator <(const Arrow &a, const Arrow &b)
{
    return (std::get<1>(a) < std::get<1>(b))
            || (std::get<2>(a) < std::get<2>(b))
            || (std::get<0>(a).x() < std::get<0>(b).x())
            || (std::get<0>(a).y() < std::get<0>(b).y());
}

inline int factorial(int n)
{
    return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

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
    QTest::addRow("zero 2x2") << Arrows{} << QSize(2, 2) << QString(zero2x2);
    QTest::addRow("zero 3x2") << Arrows{} << QSize(3, 2) << QString(zero3x2);

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

    const char *oneFromOtherEnd =
            "┌─→.\n"
            "│...\n"
            "│...\n";
    const char *oneFromOtherCenter =
            "↑..\n"
            "├─→\n"
            "│..\n";
    const char *oneFromOtherStart =
            "↑..\n"
            "│..\n"
            "└─→\n";
    const char *oneOtherEndOverlap =
            "─┬→\n"
            ".│.\n"
            ".│.\n";
    const char *oneOtherCenterOverlap =
            "─┼→\n"
            ".│.\n"
            ".│.\n";
    const char *fourCenterOverlaps =
            "↑↑.\n"
            "├┼→\n"
            "└┴→\n";
    const char *oneToDiffs =
            "...\n"
            "...\n"
            "──┘\n";
    const char *oneToDiffs2 =
            "...\n"
            "..↑\n"
            "──┘\n";
    QTest::addRow("start ending") << Arrows{Arrow({0, 0}, 3, Qt::Vertical), Arrow({0, 2}, 3, Qt::Horizontal)} << QSize(4, 3) << QString(oneFromOtherEnd);
//    QTest::addRow("center to end overlap") << Arrows{Arrow({1, 0}, 3, Qt::Vertical), Arrow({0, 2}, 3, Qt::Horizontal)} << QSize(3, 3) << QString(oneOtherEndOverlap);
//    QTest::addRow("center to center overlap") << Arrows{Arrow({1, 0}, 4, Qt::Vertical), Arrow({0, 2}, 3, Qt::Horizontal)} << QSize(3, 3) << QString(oneOtherCenterOverlap);
//    QTest::addRow("start center") << Arrows{Arrow({0, 0}, 3, Qt::Vertical), Arrow({0, 1}, 3, Qt::Horizontal)} << QSize(3, 3) << QString(oneFromOtherCenter);
//    QTest::addRow("start beginning") << Arrows{Arrow({0, 0}, 3, Qt::Vertical), Arrow({0, 0}, 3, Qt::Horizontal)} << QSize(3, 3) << QString(oneFromOtherStart);
//    QTest::addRow("four center overlaps") << Arrows{Arrow({0, 0}, 3, Qt::Vertical), Arrow({0, 0}, 3, Qt::Horizontal), Arrow({1, 0}, 3, Qt::Vertical), Arrow({0, 1}, 3, Qt::Horizontal)} << QSize(3, 3) << QString(fourCenterOverlaps);
//    QTest::addRow("one to diffs") << Arrows{Arrow({0, 0}, 3, Qt::Horizontal), Arrow({2, 0}, 1, Qt::Vertical)} << QSize(3, 3) << QString(oneToDiffs);
//    QTest::addRow("one to diffs 2") << Arrows{Arrow({0, 0}, 3, Qt::Horizontal), Arrow({2, 0}, 2, Qt::Vertical)} << QSize(3, 3) << QString(oneToDiffs2);
}

QChar _rotate90(const QChar c)
{
    const QString f("─│└┘┌┐├┤┴┬↓→←↑");
    const QString t("│─┌└┐┘┬┴├┤←↓↑→");
    const int ind = f.indexOf(c);
    return ind < 0 ? c : t[ind];
}

QString _rotate90(const QString &s)
{
    const int w = s.indexOf('\n');
    const int h = s.size() / (w + 1);
    QString r;
    for (int y = 0; y < w; ++y) {
        for (int x = 0; x < h; ++x)
            r += _rotate90(s[(h - 1 - x) * (w + 1) + y]);
        r += "\n";
    }
    return r;
}

QPoint _rotate90(const QPoint &p, const QSize &size)
{
    return {p.y(), size.width() - 1 - p.x()};
}

void _rotate(const int n90, QSize &s)
{
    if (n90 % 2)
        s = {s.height(), s.width()};
}

void _rotate(const int n90, QSize &s, QPoint &p, int &l, Qt::Orientation &o)
{
    for (int i = 0; i < n90; ++i) {
        p = _rotate90(p, s);
        s = {s.height(), s.width()};
        l *= o == 1 ? -1 : 1;
        o = Qt::Orientation(o == Qt::Vertical ? 1 : 2);
    }
}

QString inLine(const QStringList &ss)
{
    QList<int> ws;
    QList<QStringList> lines;
    int hmax = 0;

    for (const QString &s : ss) {
        hmax = std::max(s.count("\n"), hmax);
        ws.append(s.indexOf("\n") - 1);
        lines.append(s.split("\n", QString::SkipEmptyParts));
    }

    QString res;
    for (int y = 0; y <= hmax; ++y) {
        for (int x = 0; x < lines.size(); ++x)
            res += ((lines[x].size() > y) ? lines.at(x).at(y) : QString()).leftJustified(ws[x] + 10);
        res += "\n";
    }
    return res;
}

void AsciiArrows::Tests::arrows()
{
    QFETCH(Arrows, arrows);
    QFETCH(QSize, size);
    QFETCH(QString, expected);

    int b[size.width() * size.height()];

    /// check all permutations because drawing should be order-invariant
    /// as well test it on all 4 data rotations
    for (int r = 0; r < 4; ++r) {
        int nPermuts = factorial(arrows.size());
        do {
            qDebug() << "rotation =" << r << "permuts =" << nPermuts;
            AsciiArrows::clearBuffer(b, size);
            for (const Arrow &arrow : arrows) {
                // qDebug() << "    " << std::get<0>(arrow) << (std::get<0>(arrow) + ((std::get<2>(arrow) == Qt::Horizontal) ? QPoint(std::get<1>(arrow), 0) : QPoint(0, std::get<1>(arrow))));
                AsciiArrows::drawArrow(b, QRect(QPoint(0, 0), size), std::get<0>(arrow), std::get<1>(arrow), std::get<2>(arrow));
            }
            const QString res = AsciiArrows::bufferToString(b, size);
            if (res != expected) {
                const QString show = inLine({AsciiArrows::bufferToString(b, size, 2), AsciiArrows::bufferToString(b, size, 3), expected});
                qDebug().noquote().nospace() << "\n\nResult / Result codes / Expected\n\n" << show;
            }
            QCOMPARE(res, expected);
        } while (--nPermuts && std::next_permutation(arrows.begin(), arrows.end()));

        for (Arrow &arrow : arrows) {
            QSize s = size;
            _rotate(1, s, std::get<0>(arrow), std::get<1>(arrow), std::get<2>(arrow));
        }
        size = {size.height(), size.width()};
        expected = _rotate90(expected);
    }
}

void AsciiArrows::Tests::rotate90_data()
{
    QTest::addColumn<QString>("data");
    QTest::addColumn<QString>("expected");
    const char *d1 =
            "1234\n"
            "5678\n"
            "90!@\n";
    const char *e1 =
            "951\n"
            "062\n"
            "!73\n"
            "@84\n";
    const char *d2 =
             " .─│└┘┌┐├┤┴┬↓→←↑\n";
    const char *e2 =
            " \n"
            ".\n"
            "│\n"
            "─\n"
            "┌\n"
            "└\n"
            "┐\n"
            "┘\n"
            "┬\n"
            "┴\n"
            "├\n"
            "┤\n"
            "←\n"
            "↓\n"
            "↑\n"
            "→\n";
    const char *d3 =
            ".↑┌→\n"
            ".├┘.\n"
            ".↓..\n";
    const char *e3 =
            "...\n"
            "←┬→\n"
            ".└┐\n"
            "..↓\n";
    QTest::addRow("mapping") << QString(d1) << QString(e1);
    QTest::addRow("all symbols") << QString(d2) << QString(e2);
    QTest::addRow("custom test") << QString(d3) << QString(e3);
}

void AsciiArrows::Tests::rotate90()
{
    QFETCH(QString, data);
    QFETCH(QString, expected);
    const QString res = _rotate90(data);
    QCOMPARE(res, expected);
}

void AsciiArrows::Tests::rotateArrow90_data()
{
    QTest::addColumn<Arrow>("arrow");
    QTest::addColumn<QSize>("size");
    QTest::addColumn<Arrow>("expected");
    QTest::addColumn<QSize>("sizeExpected");

    QTest::addRow("1") << Arrow{{1, 1}, 1, Qt::Vertical} << QSize(3, 3) << Arrow{{1, 1}, 1, Qt::Horizontal} << QSize(3, 3);
    QTest::addRow("2") << Arrow{{1, 1}, 2, Qt::Horizontal} << QSize(3, 3) << Arrow{{1, 1}, -2, Qt::Vertical} << QSize(3, 3);
    QTest::addRow("3") << Arrow{{1, 1}, -3, Qt::Vertical} << QSize(3, 3) << Arrow{{1, 1}, -3, Qt::Horizontal} << QSize(3, 3);
    QTest::addRow("4") << Arrow{{1, 1}, -4, Qt::Horizontal} << QSize(3, 3) << Arrow{{1, 1}, 4, Qt::Vertical} << QSize(3, 3);

    QTest::addRow("5") << Arrow{{0, 0}, 1, Qt::Vertical} << QSize(2, 30) << Arrow{{0, 1}, 1, Qt::Horizontal} << QSize(30, 2);
    QTest::addRow("6") << Arrow{{0, 3}, 1, Qt::Vertical} << QSize(30, 2) << Arrow{{3, 29}, 1, Qt::Horizontal} << QSize(2, 30);
    QTest::addRow("7") << Arrow{{1, 2}, 1, Qt::Vertical} << QSize(2, 3) << Arrow{{2, 0}, 1, Qt::Horizontal} << QSize(3, 2);
    QTest::addRow("8") << Arrow{{0, -5}, 6, Qt::Vertical} << QSize(1, 2) << Arrow{{-5, 0}, 6, Qt::Horizontal} << QSize(2, 1);
}

void AsciiArrows::Tests::rotateArrow90()
{
    QFETCH(Arrow, arrow);
    QFETCH(QSize, size);
    QFETCH(Arrow, expected);
    QFETCH(QSize, sizeExpected);

    QPoint p = std::get<0>(arrow);
    int l = std::get<1>(arrow);
    auto o = std::get<2>(arrow);

    _rotate(1, size, p, l, o);
    QCOMPARE(p, std::get<0>(expected));
    QCOMPARE(l, std::get<1>(expected));
    QCOMPARE(o, std::get<2>(expected));
    QCOMPARE(size, sizeExpected);
}
